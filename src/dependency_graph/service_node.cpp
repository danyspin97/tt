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

#include "tt/dependency_graph/service_node.hpp"

#include <algorithm>

#include "tt/dependency_graph/utils.hpp"

tt::ServiceNode::ServiceNode(tt::Service service)
    : service_(std::move(service)) {
    service_name_ = std::visit(tt::GetName, service);
}

auto tt::ServiceNode::operator==(const ServiceNode &node) const -> bool {
    return node.service_name_ == service_name_;
}

auto tt::ServiceNode::name() const -> const std::string & { return service_name_; }

auto tt::ServiceNode::service() const -> tt::Service { return service_; }

void tt::ServiceNode::AddDependant() { dependants_++; }

auto tt::ServiceNode::HasDependants() const -> bool { return dependants_ != 0; }

void tt::ServiceNode::RemoveDependant() { dependants_--; }

void tt::ServiceNode::Dump(std::ostream &oss) const {
    oss << "Service: " << service_;
    oss << "\nService name: " << service_name_;
    oss << "\nDependants: " << dependants_;
}

auto operator<<(std::ostream &oss, const tt::ServiceNode &node) -> std::ostream & {
    node.Dump(oss);
    return oss;
}
