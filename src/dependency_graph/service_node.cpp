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

#include "tt/dependency_graph/service_node.hpp"

#include <ostream> // for operator<<, ostream, basic_...
#include <utility> // for move

#include "tt/dependency_graph/utils.hpp" // for GetName

tt::ServiceNode::ServiceNode(tt::Service service)
    : service_(std::move(service)) {
    service_name_ = std::visit(tt::GetName, service);
}

auto tt::ServiceNode::operator==(const ServiceNode &node) const -> bool {
    return node.service_name_ == service_name_;
}

auto tt::ServiceNode::name() const -> const std::string & {
    return service_name_;
}

auto tt::ServiceNode::service() const -> const tt::Service & {
    return service_;
}

auto tt::ServiceNode::dependants() const -> const std::vector<std::string> & {
    return dependants_;
}

void tt::ServiceNode::AddDependant(const std::string &dependant_name) {
    dependants_.push_back(dependant_name);
}

auto tt::ServiceNode::HasDependants() const -> bool {
    return !dependants_.empty();
}

void tt::ServiceNode::RemoveDependant(const std::string &dependant_name) {
    dependants_.erase(
        std::remove(dependants_.begin(), dependants_.end(), dependant_name));
}

void tt::ServiceNode::Dump(std::ostream &oss) const {
    oss << "Service: " << service_;
    oss << "\nService name: " << service_name_;
    oss << "\nDependants: ";
    std::copy(dependants_.begin(), dependants_.end(),
              std::ostream_iterator<std::string>(oss, ", "));
}

auto operator<<(std::ostream &oss, const tt::ServiceNode &node)
    -> std::ostream & {
    node.Dump(oss);
    return oss;
}
