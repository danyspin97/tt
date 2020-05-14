/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include "tt/dependency_graph/dependency_graph_handler.hpp"

#include "tt/dependency_graph/dependency_checker.hpp"
#include "tt/dependency_graph/service_node.hpp"
#include "tt/exception.hpp"

void tt::DependencyGraphHandler::AddServices(
    const std::vector<std::string> &services_to_enable,
    const std::vector<std::shared_ptr<Service>> &services) {
    auto index = graph_.AddNodes(services);
    graph_.AddEnabledServices(services_to_enable);
    graph_.PopulateDependant(services_to_enable);
    tt::DependencyChecker::Validate(graph_, index);

    // Assume that all services passed are dependencies of services_to_enable
    // assert(UnusedServices::GetUnusedServices(graph_).empty());
}

void tt::DependencyGraphHandler::RemoveServices(
    const std::vector<std::string> &services) {
    for (const auto &service : services) {
        if (!graph_.IsServiceEnabled(service)) {
            throw tt::Exception("service '" + service +
                                "' is not enabled, cannot remove");
        }
    }

    // graph_.RemoveEnabledServices(services);
    // const auto unused_services = UnusedServices::GetUnusedServices(graph_);
    // graph_.RemoveServices(unused_services);
}
