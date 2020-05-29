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

#include <algorithm>
#include <cassert>

#include "tt/data/service.hpp"
#include "tt/dependency_graph/service_node.hpp"
#include "tt/dependency_graph/utils.hpp"
#include "tt/exception.hpp"
#include "tt/parser/service/dependency_reader.hpp"

void tt::DependencyGraph::AddServices(
    const std::vector<std::string> &services_to_enable,
    const std::vector<tt::Service> &services) {
    auto index = AddNodes(services);
    AddEnabledServices(services_to_enable);
    PopulateDependant(services_to_enable);
    ValidateDependencies(index);
}

size_t tt::DependencyGraph::AddNodes(const std::vector<tt::Service> &services) {
    size_t ret_index = nodes_.size();
    size_t current_index = ret_index;
    nodes_.reserve(services.size() + nodes_.size());
    for (auto &&service : services) {
        auto get_name = [](auto &service) { return service.name(); };
        const auto name = std::visit(get_name, service);
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
        auto &node = GetNodeFromName(service_name);
        ForEachDependencyOfNode(
            node, [&](auto &dep_node) { dep_node.AddDependant(); });
    }
}

void tt::DependencyGraph::ValidateDependencies(size_t starting_index) {
    auto itr = nodes_.begin();
    advance(itr, starting_index);
    for (; itr != nodes_.end(); ++itr) {
        tt::ForEachDependencyOfService(itr->service(), [&](auto &dep) {
            if (!HasService(dep)) {
                throw tt::Exception("Dep not respected");
            }
        });
    }
}

void tt::DependencyGraph::RemoveServices(
    const std::vector<std::string> &services) {
    for (const auto &service : services) {
        if (!IsServiceEnabled(service)) {
            throw tt::Exception("service '" + service +
                                "' is not enabled, cannot remove");
        }
    }

    RemoveEnabledServices(services);
    UpdateDependants();
    RemoveUnusedServices();
}

void tt::DependencyGraph::AddEnabledServices(
    const std::vector<std::string> &services_to_enable) {
    enabled_services_.insert(services_to_enable.begin(),
                             services_to_enable.end());
}

void tt::DependencyGraph::RemoveEnabledServices(
    const std::vector<std::string> &services) {
    for (auto &&service : services) {
        size_t elem_deleted = enabled_services_.erase(service);
        assert(elem_deleted != 0);
    }
}

void tt::DependencyGraph::UpdateDependants() {
    for (auto &node : nodes_) {
        if (!IsNodeRequired(node)) {
            UpdateDependantOfNode(node);
        }
    }
}

void tt::DependencyGraph::UpdateDependantOfNode(const ServiceNode &node) {
    ForEachDependencyOfNode(node, [&](ServiceNode &dep_node) {
        dep_node.RemoveDependant();
        if (!IsNodeRequired(dep_node)) {
            UpdateDependantOfNode(dep_node);
        }
    });
}

void tt::DependencyGraph::RemoveUnusedServices() {
    nodes_.erase(std::remove_if(nodes_.begin(), nodes_.end(),
                                [*this](const auto &node) {
                                    return !IsNodeRequired(node);
                                }),
                 nodes_.end());
}

template <typename Func>
void tt::DependencyGraph::ForEachDependencyOfNode(const ServiceNode &node,
                                                  Func function) {
    tt::ForEachDependencyOfService(node.service(), [&](auto &dep_name) {
        auto &dep_node = GetNodeFromName(dep_name);
        function(dep_node);
    });
}

bool tt::DependencyGraph::IsNodeRequired(const tt::ServiceNode &node) const {
    auto itr = enabled_services_.find(node.name());
    return itr == enabled_services_.end() || node.HasDependants();
}

tt::ServiceNode &tt::DependencyGraph::GetNodeFromName(const std::string &name) {
    const auto index = name_to_index_.at(name);
    return nodes_.at(index);
}

bool tt::DependencyGraph::HasService(std::string_view service) const {
    auto itr = name_to_index_.find(service);
    return itr != name_to_index_.end();
}

bool tt::DependencyGraph::IsServiceEnabled(const std::string &service) const {
    auto itr = enabled_services_.find(service);
    return itr != enabled_services_.end();
}

const std::set<std::string_view> &
tt::DependencyGraph::enabled_services() const {
    return enabled_services_;
}

const std::vector<tt::ServiceNode> &tt::DependencyGraph::nodes() const {
    return nodes_;
}
