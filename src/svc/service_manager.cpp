/*
 * Copyright (c) 2020 Danilo Spinella <oss@danyspin97.org>.
 *
 * This file is part of tt
 * (see https://github.com/danyspin97/tt).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tt/svc/service_manager.hpp"

#include <future> // for future

#include "tt/dependency_graph/dependency_reader.hpp" // for DependencyReader
#include "tt/dependency_graph/utils.hpp"             // for GetName
#include "tt/supervision/longrun_supervisor_launcher.hpp" // for LongrunSu...
#include "tt/supervision/oneshot_supervisor.hpp" // for OneshotSupervisor

tt::ServiceManager::ServiceManager(DependencyGraph &&graph,
                                   std::shared_ptr<Dirs> dirs)
    : graph_(std::move(graph)), dirs_(std::move(dirs)), logger_registry_(dirs_),
      status_manager_(graph_.GetActiveServices(), logger_registry_),
      longrun_launcher_(dirs_) {}

void tt::ServiceManager::StartAllServices() {
    std::vector<std::future<void>> start_scripts_running;
    // TODO: Calculate an optimal order of services to start
    for (const auto &service : graph_.services()) {
        const auto &service_name = std::visit(tt::GetName, service.get());
        start_scripts_running.emplace_back(
            std::async(std::launch::async, &tt::ServiceManager::StartService,
                       this, service_name, service));
    }

    for (const auto &future : start_scripts_running) {
        future.wait();
    }

    logger_registry_.Flush();
}

void tt::ServiceManager::StopAllServices() {
    std::vector<std::future<void>> stop_scripts_running;
    auto up_services = status_manager_.GetUpServices();
    stop_scripts_running.reserve(up_services.size());
    for (const auto &service_name : up_services) {
        const auto &service = graph_.GetServiceByName(service_name);
        stop_scripts_running.emplace_back(
            std::async(std::launch::async, &tt::ServiceManager::StopService,
                       this, service_name, service));
    }

    for (const auto &future : stop_scripts_running) {
        future.wait();
    }
    logger_registry_.Flush();
}

void tt::ServiceManager::StartService(const std::string &service_name,
                                      const Service &service) {

    DependencyReader dep_reader{};
    std::visit(std::ref(dep_reader), service);
    auto deps = dep_reader.dependencies();

    auto deps_satisfied = true;
    for (const auto &dep : deps) {
        if (!status_manager_.WaitOnServiceStart(dep)) {
            deps_satisfied = false;
            break;
        }
    }

    if (!deps_satisfied) {
        status_manager_.ChangeStatusOfService(service_name,
                                              ServiceStatus::Down);
        return;
    }

    std::visit(
        [this, &service_name](const auto &service) {
            if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                         Oneshot>) {
                const auto &oneshot = static_cast<Oneshot>(service);
                OneshotSupervisor supervisor{
                    oneshot, logger_registry_.GetOneshotLogger(service_name)};
                status_manager_.ChangeStatusOfService(service_name,
                                                      ServiceStatus::Starting);
                bool success = supervisor.Start();
                auto new_status =
                    success ? ServiceStatus::Up : ServiceStatus::Down;
                status_manager_.ChangeStatusOfService(service_name, new_status);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                                Longrun>) {
                const auto &longrun = static_cast<Longrun>(service);
                longrun_launcher_.Launch(longrun);
            }
        },
        service);
}

void tt::ServiceManager::StopService(const std::string &service_name,
                                     const Service &service) {
    const auto &dependants = graph_.GetDependantsOfService(service_name);
    for (const auto &dependant : dependants) {
        status_manager_.WaitOnServiceDown(dependant);
    }

    std::visit(
        [this, &service_name](const auto &service) {
            if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                         Oneshot>) {
                const auto &oneshot = static_cast<Oneshot>(service);
                if (oneshot.stop()) {
                    OneshotSupervisor supervisor{
                        oneshot,
                        logger_registry_.GetOneshotLogger(service_name)};
                    status_manager_.ChangeStatusOfService(
                        service_name, ServiceStatus::Stopping);
                    supervisor.Stop();
                }
                status_manager_.ChangeStatusOfService(service_name,
                                                      ServiceStatus::Down);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                                Longrun>) {
                longrun_launcher_.Close(service_name);
            }
        },
        service);
}
