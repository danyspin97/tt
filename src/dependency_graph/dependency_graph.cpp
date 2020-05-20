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
    size_t ret_index = services_.size();
    size_t current_index = ret_index;
    services_.reserve(services.size() + services_.size());
    for (auto &&service : services) {
        auto get_name = [](auto &service) { return service.name(); };
        const auto name = std::visit(get_name, service);
        if (!IsServiceEnabled(name)) {
            services_.emplace_back(service);
            name_to_index_[name] = current_index;
            current_index++;
        }
    }
    return ret_index;
}

void tt::DependencyGraph::PopulateDependant(
    const std::vector<std::string> &services) {
    for (const auto &service_name : services) {
        auto &node = GetServiceFromName(service_name);
        auto service = node.service();
        tt::DependencyReader dep_reader;
        std::visit(dep_reader, service);
        const auto deps = dep_reader.dependencies();
        for (auto &&dep : deps) {
            auto &dep_node = GetServiceFromName(dep);
            dep_node.AddDependant();
        }
    }
}

void tt::DependencyGraph::ValidateDependencies(size_t starting_index) const {
    auto itr = services_.begin();
    advance(itr, starting_index);
    for (; itr != services_.end(); ++itr) {
        tt::DependencyReader dep_reader;
        std::visit(dep_reader, (*itr).service());
        const auto deps = dep_reader.dependencies();
        for (const auto &dep : deps) {
            if (!IsServiceActive(dep)) {
                throw tt::Exception("Dep not respected");
            }
        }
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
    for (auto &&service : services_to_enable) {
        enabled_services_.insert(service);
    }
}

void tt::DependencyGraph::RemoveEnabledServices(
    const std::vector<std::string> &services) {
    for (auto &&service : services) {
        size_t elem_deleted = enabled_services_.erase(service);
        assert(elem_deleted != 0);
    }
}

void tt::DependencyGraph::UpdateDependants() {
    for (auto &service : services_) {
        if (!IsServiceUsed(service)) {
            UpdateDependant(service);
        }
    }
}

void tt::DependencyGraph::UpdateDependant(const ServiceNode &node) {
    tt::DependencyReader dep_reader;
    std::visit(dep_reader, node.service());
    const auto deps = dep_reader.dependencies();
    for (auto &&dep : deps) {
        auto &dep_node = GetServiceFromName(dep);
        dep_node.RemoveDependant();
        if (!IsServiceUsed(dep_node)) {
            UpdateDependant(dep_node);
        }
    }
}

void tt::DependencyGraph::RemoveUnusedServices() {
    services_.erase(std::remove_if(services_.begin(), services_.end(),
                                   [*this](const auto &node) {
                                       return !IsServiceUsed(node);
                                   }),
                    services_.end());
}

bool tt::DependencyGraph::IsServiceUsed(const tt::ServiceNode &node) const {
    auto itr = enabled_services_.find(node.name());
    return !(itr == enabled_services_.end() && node.HasDependants());
}

tt::ServiceNode &
tt::DependencyGraph::GetServiceFromName(const std::string &name) {
    const auto index = name_to_index_.at(name);
    return services_.at(index);
}

bool tt::DependencyGraph::IsServiceActive(const std::string &service) const {
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

const std::vector<tt::ServiceNode> &tt::DependencyGraph::services() const {
    return services_;
}
