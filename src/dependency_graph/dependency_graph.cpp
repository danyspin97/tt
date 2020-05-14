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

#include "tt/dependency_graph/dependency_graph.hpp"

#include "tt/data/service.hpp"
#include "tt/dependency_graph/service_node.hpp"
#include "tt/parser/service/dependency_reader.hpp"

size_t tt::DependencyGraph::AddNodes(
    const std::vector<std::shared_ptr<Service>> &services) {
    size_t ret_index = nodes_.size();
    size_t current_index = ret_index;
    nodes_.reserve(services.size() + nodes_.size());
    for (auto &&service : services) {
        const auto name = service->name();
        if (!IsServiceEnabled(name)) {
            nodes_.emplace_back(service);
            name_to_index_[name] = current_index;
            current_index++;
        }
    }
    return ret_index;
}

void tt::DependencyGraph::PopulateDependant(
    const std::vector<std::string> &services) {
    for (const auto &service_name : services) {
        auto node = GetServiceFromName(service_name);
        auto service = node.service();
        tt::DependencyReader dep_reader;
        service->Accept(dep_reader);
        const auto deps = dep_reader.dependencies();
        for (auto &&dep : deps) {
            auto dep_node = GetServiceFromName(dep);
            dep_node.AddDependant(service);
        }
    }
}

void tt::DependencyGraph::AddEnabledServices(
    const std::vector<std::string> &services_to_enable) {
    for (auto &&service : services_to_enable) {
        enabled_services_.insert(service);
    }
}

const tt::ServiceNode &
tt::DependencyGraph::GetServiceFromName(const std::string &name) const {
    const auto index = name_to_index_.at(name);
    return nodes_.at(index);
}

bool tt::DependencyGraph::IsServiceActive(const std::string &service) const {
    auto itr = name_to_index_.find(service);
    return itr != name_to_index_.end();
}

bool tt::DependencyGraph::IsServiceEnabled(const std::string &service) const {
    auto itr = enabled_services_.find(service);
    return itr != enabled_services_.end();
}

const std::set<std::string> &tt::DependencyGraph::enabled_services() const {
    return enabled_services_;
}

const std::vector<tt::ServiceNode> &tt::DependencyGraph::services() const {
    return nodes_;
}
