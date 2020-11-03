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

#include <string> // for string

#include "tt/data/service.hpp"                      // for Service
#include "tt/dependency_graph/dependency_graph.hpp" // for DependencyGraph
#include "tt/log/service_logger_registry.hpp"       // for ServiceLoggerRegistry
#include "tt/supervision/longrun_supervisor_launcher.hpp" // for LongrunSup...
#include "tt/svc/service_status_manager.hpp" // for ServiceStatusManager

namespace tt {

class ServiceManager {
public:
    ServiceManager(DependencyGraph &&graph, std::shared_ptr<Dirs> dirs);

    void StartAllServices();
    void StopAllServices();

private:
    void StartService(const std::string &service_name, const Service &service);
    void StopService(const std::string &service_name, const Service &service);

    DependencyGraph graph_;
    ServiceStatusManager status_manager_;
    std::shared_ptr<Dirs> dirs_;
    // ServiceLoggerRegistry needs dirs to construct
    ServiceLoggerRegistry logger_registry_;
    LongrunSupervisorLauncher longrun_launcher_;
};

} // namespace tt
