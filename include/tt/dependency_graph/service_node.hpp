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

namespace tt {

class Service;

class ServiceNode {
public:
    explicit ServiceNode(std::shared_ptr<Service> service);
    std::shared_ptr<Service> service();

    void AddDependant(const std::shared_ptr<Service> &service_to_add);

private:
    std::shared_ptr<Service> service_;
    std::vector<std::shared_ptr<Service>> dependant_;
    std::vector<std::shared_ptr<Service>> optional_dependencies_;
};

} // namespace tt
