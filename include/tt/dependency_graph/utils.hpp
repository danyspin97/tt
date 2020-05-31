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

#include "tt/data/service.hpp"
#include "tt/dependency_graph/dependency_reader.hpp"

namespace tt {

class DependencyGraph;

void AddTestDependenciesToGraph(DependencyGraph &graph,
                                const std::vector<std::string> &filenames);

template <typename Func>
void ForEachDependencyOfService(const Service &service, Func function) {
    tt::DependencyReader dep_reader;
    std::visit(dep_reader, service);
    const auto deps = dep_reader.dependencies();
    for (auto &&dep_name : deps) {
        function(dep_name);
    }
}

} // namespace tt
