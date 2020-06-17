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
    : service_(std::move(service)), dependants_(0) {
    service_name_ = std::visit(tt::GetName, service);
}

const std::string &tt::ServiceNode::name() const { return service_name_; }

tt::Service tt::ServiceNode::service() const { return service_; }

void tt::ServiceNode::AddDependant() { dependants_++; }

bool tt::ServiceNode::HasDependants() const { return dependants_ != 0; }

void tt::ServiceNode::RemoveDependant() { dependants_--; }
