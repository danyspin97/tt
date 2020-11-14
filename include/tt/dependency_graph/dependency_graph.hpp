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

#include <cstddef> // for size_t
#include <map>     // for map
#include <set>     // for set
#include <string>  // for string
#include <vector>  // for vector

#include "bitsery/ext/std_map.h" // for StdMap
#include "bitsery/ext/std_set.h" // for StdSet

#include "tt/data/service.hpp"                  // for Service
#include "tt/dependency_graph/service_node.hpp" // for ServiceNode

namespace bitsery {
class Access;
} // namespace bitsery

namespace tt {

class DependencyGraph {
public:
    [[nodiscard]] auto enabled_services() const
        -> const std::set<std::string> &;
    [[nodiscard]] auto services() const
        -> std::vector<std::reference_wrapper<const Service>>;

    void AddServices(const std::vector<std::string> &services_to_enable,
                     std::vector<tt::Service> &services);
    [[nodiscard]] auto GetActiveServices() const -> std::vector<std::string>;
    [[nodiscard]] auto GetServiceByName(const std::string &name)
        -> const Service &;
    [[nodiscard]] auto GetDependantsOfService(const std::string &service_name)
        -> const std::vector<std::string> &;
    [[nodiscard]] auto HasService(const std::string &service) const -> bool;
    [[nodiscard]] auto IsServiceEnabled(const std::string &service) const
        -> bool;
    void RemoveServices(const std::vector<std::string> &services);

private:
    auto AddNewNodes(std::vector<Service> &&services) -> size_t;
    void AddEnabledServices(const std::vector<std::string> &services_to_enable);
    void CheckDepenciesAreFullfilled(size_t starting_index);
    void CheckGraphIsAcyclic(const std::vector<std::string> &enabled_services);
    template <typename Func>
    void ForEachDependencyOfNode(const ServiceNode &node, Func function);
    [[nodiscard]] auto GetNodeFromName(const std::string &name)
        -> ServiceNode &;
    [[nodiscard]] auto IsNodeRequired(const ServiceNode &node) const -> bool;
    void PopulateDependant(const std::vector<std::string> &services);
    void RemoveEnabledServices(const std::vector<std::string> &services);
    void RemoveService(const ServiceNode &node);
    void RemoveUnusedServices();
    auto ReplaceExistingNodes(std::vector<tt::Service> &services) -> bool;
    void UpdateDependantOfNode(const ServiceNode &node);
    void UpdateDependants();

    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.ext(
            enabled_services_,
            bitsery::ext::StdSet{enabled_services_.max_size()},
            [](S &serializer, std::string &name) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(name, name.max_size());
            });
        serializer.ext(
            name_to_index_, bitsery::ext::StdMap{name_to_index_.max_size()},
            [](S &serializer, std::string &name, size_t &index) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(name, name.max_size());
                serializer.template value<sizeof(size_t)>(index);
            });
        serializer.container(nodes_, nodes_.max_size());
    }

    std::set<std::string> enabled_services_;
    std::map<std::string, size_t> name_to_index_;
    std::vector<ServiceNode> nodes_;
}; // namespace tt

} // namespace tt
