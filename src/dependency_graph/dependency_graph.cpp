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

#include "tt/dependency_graph/dependency_graph.hpp"

#include <algorithm>   // for for_each, remove_if
#include <cassert>     // for assert
#include <functional>  // for function
#include <iterator>    // for back_insert_iterator
#include <string_view> // for string_view, operator<
#include <variant>     // for holds_alternative

#include "tt/data/service.hpp"                  // for Service
#include "tt/dependency_graph/service_node.hpp" // for ServiceNode
#include "tt/dependency_graph/utils.hpp"        // for ForEachDependencyOfS...
#include "tt/exception.hpp"                     // for Exception

void tt::DependencyGraph::AddServices(
    const std::vector<std::string> &services_to_enable,
    std::vector<tt::Service> &services) {
    auto existing_services = SplitExistingServices(services);
    auto index = AddNodes(services.begin(), existing_services);
    auto nodes_replaced =
        ReplaceExistingNodes(existing_services, services.end());
    AddEnabledServices(services_to_enable);
    CheckDepenciesAreFullfilled(nodes_replaced ? 0 : index);
    CheckGraphIsAcyclic(services_to_enable);
    PopulateDependant(services_to_enable);
}

auto tt::DependencyGraph::SplitExistingServices(std::vector<Service> &services)
    -> std::vector<Service>::iterator {
    return std::remove_if(services.begin(), services.end(),
                          [this](const auto &service) {
                              return HasService(std::visit(GetName, service));
                          });
}

auto tt::DependencyGraph::AddNodes(std::vector<Service>::iterator begin,
                                   std::vector<Service>::iterator end)
    -> size_t {
    size_t ret_index = nodes_.size();
    size_t current_index = ret_index;
    nodes_.reserve(nodes_.size() + std::distance(begin, end));
    std::for_each(begin, end, [this, &current_index](auto &service) {
        assert(!std::holds_alternative<std::monostate>(service));
        const auto name = std::visit(tt::GetName, service);
        // Assert that this is indeed a new service
        assert(!HasService(name));
        assert(nodes_.size() == current_index);
        nodes_.emplace_back(std::move(service));
        name_to_index_.emplace(name, current_index);
        current_index++;
    });
    return ret_index;
}

auto tt::DependencyGraph::ReplaceExistingNodes(
    std::vector<Service>::iterator begin, std::vector<Service>::iterator end)
    -> bool {
    bool replaced = false;
    std::for_each(begin, end, [this, &replaced](auto &service) {
        const auto &service_name = std::visit(GetName, service);
        assert(HasService(service_name));
        auto node_index = name_to_index_.at(service_name);
        auto old_node = nodes_.at(node_index);
        // The new node contains the same service as the
        // old one, remove it from the services list
        if (old_node.service() == service) {
            return;
        }

        ForEachDependencyOfNode(old_node,
                                [this, service_name](ServiceNode &dep_node) {
                                    dep_node.RemoveDependant(service_name);
                                });

        ServiceNode new_node(service);
        nodes_.at(node_index) = std::move(new_node);
        PopulateDependant({service_name});
        replaced = true;
    });
    return replaced;
}

void tt::DependencyGraph::PopulateDependant(
    const std::vector<std::string> &services) {
    for (const auto &service_name : services) {
        const auto &node = GetNodeFromName(service_name);
        ForEachDependencyOfNode(node, [service_name](auto &dep_node) {
            dep_node.AddDependant(service_name);
        });
    }
}

void tt::DependencyGraph::CheckDepenciesAreFullfilled(size_t starting_index) {
    auto itr = nodes_.begin();
    advance(itr, starting_index);
    for (; itr != nodes_.end(); ++itr) {
        tt::ForEachDependencyOfService(
            itr->service(), [this](const std::string &dep) {
                if (!HasService(dep)) {
                    throw tt::Exception("Dependency '" + dep +
                                        "' could not be found");
                }
            });
    }
}

void tt::DependencyGraph::CheckGraphIsAcyclic(
    const std::vector<std::string> &enabled_services) {
    enum class Color { White = 0, Gray = 1, Black = 2 };

    std::map<std::string_view, Color> colors{};
    std::function<void(ServiceNode)> visit =
        [this, &visit, &colors](const ServiceNode &node) {
            colors[node.name()] = Color::Gray;

            ForEachDependencyOfNode(
                node, [visit, &colors](const ServiceNode &dep_node) {
                    switch (colors.at(dep_node.name())) {
                    case Color::White:
                        visit(dep_node);
                        break;
                    case Color::Gray:
                        throw Exception("Cycle found while calculating the "
                                        "dependencies of the services");
                    default:
                        assert(false);
                    }
                });

            colors[node.name()] = Color::Black;
        };

    std::for_each(begin(nodes_), end(nodes_),
                  [&colors](const ServiceNode &node) {
                      colors.emplace(node.name(), Color::White);
                  });
    for (const auto &service_name : enabled_services) {
        if (colors.at(service_name) == Color::White) {
            const auto &node = GetNodeFromName(service_name);
            visit(node);
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
    for (const auto &node : nodes_) {
        if (!IsNodeRequired(node)) {
            UpdateDependantOfNode(node);
        }
    }
}

void tt::DependencyGraph::UpdateDependantOfNode(const ServiceNode &node) {
    const auto &service_name = node.name();
    ForEachDependencyOfNode(node, [this, service_name](ServiceNode &dep_node) {
        dep_node.RemoveDependant(service_name);
        if (!IsNodeRequired(dep_node)) {
            UpdateDependantOfNode(dep_node);
        }
    });
}

void tt::DependencyGraph::RemoveUnusedServices() {
    nodes_.erase(std::remove_if(nodes_.begin(), nodes_.end(),
                                [this](const auto &node) {
                                    return !IsNodeRequired(node);
                                }),
                 nodes_.end());
}

template <typename Func>
void tt::DependencyGraph::ForEachDependencyOfNode(const ServiceNode &node,
                                                  Func function) {
    tt::ForEachDependencyOfService(
        node.service(), [this, function](auto &dep_name) {
            auto &dep_node = GetNodeFromName(dep_name);
            function(dep_node);
        });
}

auto tt::DependencyGraph::IsNodeRequired(const tt::ServiceNode &node) const
    -> bool {
    auto itr = enabled_services_.find(node.name());
    return itr != enabled_services_.end() || node.HasDependants();
}

auto tt::DependencyGraph::GetNodeFromName(const std::string &name)
    -> tt::ServiceNode & {
    assert(name_to_index_.find(name) != end(name_to_index_));
    const auto index = name_to_index_.at(name);
    return nodes_.at(index);
}

auto tt::DependencyGraph::HasService(const std::string &service) const -> bool {
    auto itr = name_to_index_.find(service);
    return itr != name_to_index_.end();
}

auto tt::DependencyGraph::IsServiceEnabled(const std::string &service) const
    -> bool {
    auto itr = enabled_services_.find(service);
    return itr != enabled_services_.end();
}

auto tt::DependencyGraph::GetActiveServices() const
    -> std::vector<std::string> {
    std::vector<std::string> services;
    std::transform(name_to_index_.begin(), name_to_index_.end(),
                   std::back_inserter(services),
                   [](auto &pair) { return pair.first; });
    return services;
}

auto tt::DependencyGraph::GetServiceByName(const std::string &name)
    -> const Service & {
    const auto &node = GetNodeFromName(name);
    return node.service();
}

auto tt::DependencyGraph::GetDependantsOfService(
    const std::string &service_name) -> const std::vector<std::string> & {
    return GetNodeFromName(service_name).dependants();
}

auto tt::DependencyGraph::enabled_services() const
    -> const std::set<std::string> & {
    return enabled_services_;
}

auto tt::DependencyGraph::services() const
    -> std::vector<std::reference_wrapper<const tt::Service>> {
    std::vector<std::reference_wrapper<const tt::Service>> services;
    for (const auto &node : nodes_) {
        services.emplace_back(std::cref(node.service()));
    }
    return services;
}
