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

#include <memory>
#include <vector>

#include "tt/data/service.hpp"

namespace tt {

class ServiceNode;

class ServiceNode {
public:
    explicit ServiceNode(Service service);
    [[nodiscard]] Service service() const;
    [[nodiscard]] const std::string &name() const;

    void AddDependant();
    [[nodiscard]] bool HasDependants() const;
    void RemoveDependant();

private:
    Service service_;
    uint_fast8_t dependants_;
    std::string service_name_;
};

} // namespace tt
