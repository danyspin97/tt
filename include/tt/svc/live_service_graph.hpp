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

#pragma once

#include <shared_mutex> // for shared_mutex
#include <string>       // for string

#include "tt/data/service.hpp"                      // for Service
#include "tt/dependency_graph/dependency_graph.hpp" // for DependencyGraph
#include "tt/log/service_logger_registry.hpp"       // for ServiceLoggerRegistry
#include "tt/supervision/longrun_supervisor_launcher.hpp" // for LongrunSup...
#include "tt/supervision/types.hpp"                       // for ScriptStatus
#include "tt/svc/live_service.hpp"                        // for LiveService

namespace tt {

class LiveService;

class LiveServiceGraph {
public:
    LiveServiceGraph(DependencyGraph &&graph, std::shared_ptr<Dirs> dirs);

    [[nodiscard]] auto live_services() const
        -> std::vector<std::reference_wrapper<const LiveService>>;
    [[nodiscard]] auto services() const
        -> std::vector<std::reference_wrapper<const Service>>;

    void StartAllServices();
    void StopAllServices();

    [[nodiscard]] auto HasService(const std::string &service) const -> bool;

    void ChangeStatusOfService(const std::string &service,
                               ServiceStatus new_status);
    [[nodiscard]] auto GetUpServices() const -> std::vector<std::string>;

    [[nodiscard]] auto WaitOnServiceStart(const std::string &service) -> bool;
    void WaitOnServiceDown(const std::string &service);

    void Update(DependencyGraph &&new_dependency_graph);

    auto GetLiveServiceFromName(const std::string &name) -> LiveService &;

    // Start a service and all its dependency
    auto StartService(const std::string &service_name)
        -> tl::expected<ScriptStatus, std::string>;
    // Same as stop service
    auto StopService(const std::string &service_name)
        -> tl::expected<void, std::string>;

private:
    // Start a single service, check its status is valid before trying to
    // putting it up
    void StartSingleService(LiveService &service);
    // Same as StartSingleService
    void StopSingleService(LiveService &service);

    static inline std::shared_mutex mutex_;
    std::map<std::string, size_t> name_to_index_;
    std::vector<LiveService> live_services_;
    std::shared_ptr<Dirs> dirs_;
    // ServiceLoggerRegistry needs dirs to construct
    ServiceLoggerRegistry logger_registry_;
    LongrunSupervisorLauncher longrun_launcher_;
};

} // namespace tt
