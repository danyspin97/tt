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

#include "tt/svc/live_service_graph.hpp"

#include <future> // for future

#include "tt/dependency_graph/dependency_reader.hpp" // for DependencyReader
#include "tt/dependency_graph/utils.hpp"             // for GetName
#include "tt/supervision/longrun_supervisor_launcher.hpp" // for LongrunSu...
#include "tt/supervision/oneshot_supervisor.hpp" // for OneshotSupervisor

tt::LiveServiceGraph::LiveServiceGraph(DependencyGraph &&graph,
                                       std::shared_ptr<Dirs> dirs)
    : dirs_(std::move(dirs)), logger_registry_(dirs_),
      longrun_launcher_(dirs_) {
    auto graph_nodes = graph.nodes();
    live_services_.reserve(graph_nodes.size());
    std::for_each(graph_nodes.begin(), graph_nodes.end(), [this](auto &node) {
        live_services_.emplace_back(std::move(node));
    });
    name_to_index_ = std::move(graph.name_to_index());
}

void tt::LiveServiceGraph::Update(DependencyGraph &&new_dependency_graph) {
    auto name_to_index = std::move(new_dependency_graph.name_to_index());
    auto existing_services = name_to_index_.begin();
    auto new_services = name_to_index.begin();
    std::vector<std::reference_wrapper<const std::string>> nodes_to_add;
    // Let's iterate both services map. They are alphabetically ordered
    while (existing_services != name_to_index_.end() &&
           new_services != name_to_index.end()) {
        // If the new dependency_graph has a service that isn't
        // enabled in the new_dependency_graph, it means that
        // it has been removed
        if (existing_services->first < new_services->first) {
            GetLiveServiceFromName(existing_services->first).MarkForRemoval();
            existing_services++;
            continue;
        }
        // The existing graph is missing a service
        if (existing_services->first > new_services->first) {
            nodes_to_add.emplace_back(std::cref(new_services->first));
            new_services++;
            continue;
        }

        // Both graphs have this service
        auto &live_service = GetLiveServiceFromName(existing_services->first);
        // It's faster to just change a value rather than adding a branch
        live_service.UnmarkForRemoval();
        auto &node = new_dependency_graph.nodes().at(new_services->second);
        if (live_service.node() != node) {
            live_service.AddUpdatedNode(std::move(node));
        }
        existing_services++;
        new_services++;
    }

    std::for_each(existing_services, name_to_index_.end(), [this](auto &pair) {
        GetLiveServiceFromName(pair.first).MarkForRemoval();
    });

    std::for_each(new_services, name_to_index.end(),
                  [&nodes_to_add](const auto &pair) {
                      nodes_to_add.emplace_back(std::cref(pair.first));
                  });

    if (nodes_to_add.empty()) {
        return;
    }

    // Add the new nodes to live_services_
    auto nodes = std::move(new_dependency_graph.nodes());
    auto current_index = live_services_.size();
    live_services_.reserve(current_index + nodes_to_add.size());
    for (const auto &service_name : nodes_to_add) {
        auto &node = nodes.at(name_to_index.at(service_name));
        name_to_index_.emplace(node.name(), current_index);
        live_services_.emplace_back(std::move(node));
        current_index++;
    }
}

void tt::LiveServiceGraph::StartAllServices() {
    std::vector<std::future<void>> start_scripts_running;
    // TODO: Calculate an optimal order of services to start
    for (auto &node : live_services_) {
        start_scripts_running.emplace_back(
            std::async(std::launch::async, &tt::LiveServiceGraph::StartService,
                       this, std::ref(node)));
    }

    for (const auto &future : start_scripts_running) {
        future.wait();
    }

    logger_registry_.Flush();
}

void tt::LiveServiceGraph::StopAllServices() {
    std::vector<std::future<void>> stop_scripts_running;
    auto up_services = GetUpServices();
    stop_scripts_running.reserve(up_services.size());
    for (auto &service_name : up_services) {
        auto &node = GetLiveServiceFromName(service_name);
        stop_scripts_running.emplace_back(
            std::async(std::launch::async, &tt::LiveServiceGraph::StopService,
                       this, std::ref(node)));
    }

    for (const auto &future : stop_scripts_running) {
        future.wait();
    }
}

void tt::LiveServiceGraph::StartService(LiveService &live_service) {
    ChangeStatusOfService(live_service.name(), ServiceStatus::Starting);
    DependencyReader dep_reader{};
    std::visit(std::ref(dep_reader), live_service.service());
    auto deps = dep_reader.dependencies();

    auto deps_satisfied = true;
    for (const auto &dep : deps) {
        if (!WaitOnServiceStart(dep)) {
            deps_satisfied = false;
            break;
        }
    }

    if (!deps_satisfied) {
        ChangeStatusOfService(live_service.name(), ServiceStatus::Down);
        return;
    }

    std::visit(
        [this, &live_service](const auto &service) {
            const auto &service_name = live_service.name();
            if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                         Oneshot>) {
                const auto &oneshot = static_cast<Oneshot>(service);
                OneshotSupervisor supervisor{
                    oneshot, logger_registry_.GetOneshotLogger(service_name)};
                bool success = supervisor.Start();
                auto new_status =
                    success ? ServiceStatus::Up : ServiceStatus::Down;
                ChangeStatusOfService(service_name, new_status);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                                Longrun>) {
                const auto &longrun = static_cast<Longrun>(service);
                live_service.supervisor(longrun_launcher_.Launch(longrun));
            }
        },
        live_service.service());
}

void tt::LiveServiceGraph::StopService(LiveService &live_service) {
    ChangeStatusOfService(live_service.name(), ServiceStatus::Stopping);
    const auto &dependants = live_service.node().dependants();
    for (const auto &dependant : dependants) {
        WaitOnServiceDown(dependant);
    }

    std::visit(
        [this, &live_service](const auto &service) {
            const auto &service_name = live_service.name();
            if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                         Oneshot>) {
                const auto &oneshot = static_cast<Oneshot>(service);
                if (oneshot.stop()) {
                    OneshotSupervisor supervisor{
                        oneshot,
                        logger_registry_.GetOneshotLogger(service_name)};
                    supervisor.Stop();
                }
            } else if constexpr (std::is_same_v<std::decay_t<decltype(service)>,
                                                Longrun>) {
                auto supervisor = std::move(live_service.supervisor());
                assert(supervisor);
                LongrunSupervisorLauncher::Close(*supervisor);
                // Call waitid to remove zombie process
                supervisor->get_exit_status();
            }
        },
        live_service.service());
    ChangeStatusOfService(live_service.name(), ServiceStatus::Down);
}

void tt::LiveServiceGraph::ChangeStatusOfService(const std::string &service,
                                                 ServiceStatus new_status) {
    // We are going to write into the map, do not allow other threads to read
    std::unique_lock lock{mutex_};
    auto &live_service = GetLiveServiceFromName(service);
    live_service.status(new_status);

    switch (new_status) {
    case ServiceStatus::Up:
        live_service.SignalUp();
        break;
    case ServiceStatus::Down:
        live_service.SignalDown();
        break;
    default:
        // No need to do anything here
        ;
    }
    logger_registry_.LogServiceStatus(service, new_status);
}

auto tt::LiveServiceGraph::WaitOnServiceStart(const std::string &service)
    -> bool {
    auto &live_service = GetLiveServiceFromName(service);
    live_service.WaitUp();

    // We only need the shared_lock when reading from services_ map
    std::shared_lock lock{mutex_};
    return live_service.status() == ServiceStatus::Up;
}

void tt::LiveServiceGraph::WaitOnServiceDown(const std::string &service) {
    GetLiveServiceFromName(service).WaitDown();
}

auto tt::LiveServiceGraph::GetUpServices() const -> std::vector<std::string> {
    std::shared_lock lock(mutex_);
    std::vector<std::string> up_services;
    for (const auto &live_service : live_services_) {
        if (live_service.status() == ServiceStatus::Up) {
            up_services.push_back(live_service.name());
        }
    }

    return up_services;
}

auto tt::LiveServiceGraph::GetLiveServiceFromName(const std::string &name)
    -> LiveService & {
    assert(name_to_index_.find(name) != end(name_to_index_));
    const auto index = name_to_index_.at(name);
    return live_services_.at(index);
}

auto tt::LiveServiceGraph::HasService(const std::string &service) const
    -> bool {
    auto itr = name_to_index_.find(service);
    return itr != name_to_index_.end();
}

auto tt::LiveServiceGraph::services() const
    -> std::vector<std::reference_wrapper<const tt::Service>> {
    std::vector<std::reference_wrapper<const tt::Service>> services;
    for (const auto &node : live_services_) {
        services.emplace_back(std::cref(node.service()));
    }
    return services;
}

auto tt::LiveServiceGraph::live_services() const
    -> std::vector<std::reference_wrapper<const LiveService>> {
    std::vector<std::reference_wrapper<const LiveService>> services;
    for (const auto &live_service : live_services_) {
        services.emplace_back(std::cref(live_service));
    }
    return services;
}
