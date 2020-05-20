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

#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "tt/data/service.hpp"

namespace tt {

class ServiceNode;

class DependencyGraph {
private:
    std::set<std::string_view> enabled_services_;
    std::map<std::string_view, size_t> name_to_index_;
    std::vector<ServiceNode> services_;

public:
    [[nodiscard]] const std::set<std::string_view> &enabled_services() const;
    [[nodiscard]] const std::vector<ServiceNode> &services() const;

    size_t AddNodes(const std::vector<Service> &services);
    void AddServices(const std::vector<std::string> &services_to_enable,
                     const std::vector<tt::Service> &services);
    void AddEnabledServices(const std::vector<std::string> &services_to_enable);
    [[nodiscard]] ServiceNode &GetServiceFromName(const std::string &name);
    [[nodiscard]] bool IsServiceActive(const std::string &service) const;
    [[nodiscard]] bool IsServiceEnabled(const std::string &service) const;
    [[nodiscard]] bool IsServiceUsed(const ServiceNode &node) const;
    void PopulateDependant(const std::vector<std::string> &services);
    void RemoveServices(const std::vector<std::string> &services);
    void RemoveEnabledServices(const std::vector<std::string> &services);
    void RemoveUnusedServices();
    void RemoveService(const ServiceNode &node);
    void ValidateDependencies(size_t starting_index) const;
    void UpdateDependant(const ServiceNode &node);
    void UpdateDependants();
};

} // namespace tt