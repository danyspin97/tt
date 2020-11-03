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

#include "tt/dependency_graph/dependency_reader.hpp" // for DependencyReader
#include "tt/dependency_graph/utils.hpp"             // for GetName
#include "tt/supervision/longrun_supervisor_launcher.hpp" // for LongrunSu...
#include "tt/supervision/oneshot_supervisor.hpp" // for OneshotSupervisor
#include "tt/utils/launch_async.hpp"             // for LaunchAsync

tt::ServiceManager::ServiceManager(DependencyGraph &&graph,
                                   std::shared_ptr<Dirs> dirs)
    : graph_(std::move(graph)), status_manager_(graph_.GetActiveServices()),
      dirs_(std::move(dirs)), logger_registry_(dirs_) {}

void tt::ServiceManager::StartAllServices() {
    // TODO: Calculate an optimal order of services to start
    for (const auto &service : graph_.services()) {
        const auto &service_name = std::visit(tt::GetName, service.get());
        LaunchAsync(
            [this, node]() { StartService(service_name, service); });
    }
}

void tt::ServiceManager::StartService(const std::string &service_name,
                                      const Service &service) {
    status_manager_.ChangeStatusOfService(service_name,
                                          ServiceStatus::Starting);

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
    if constexpr (!std::is_same_v<std::decay_t<decltype(service)>, Oneshot>) {
        const auto &oneshot = std::get<Oneshot>(service);
        OneshotSupervisor spawn{
            oneshot, logger_registry_.GetOneshotLogger(service_name)};
        bool success = spawn.Start();
        auto new_status = success ? ServiceStatus::Up : ServiceStatus::Down;
        status_manager_.ChangeStatusOfService(service_name, new_status);
    } else if constexpr (!std::is_same_v<std::decay_t<decltype(service)>,
                                         Oneshot>) {
        const auto &longrun = std::get<Longrun>(service);
        LongrunSupervisorLauncher launcher{longrun, dirs_};
        launcher.Launch();
    }
}
