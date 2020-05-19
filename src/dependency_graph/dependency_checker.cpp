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

#include "tt/dependency_graph/dependency_checker.hpp"

#include "tt/data/service.hpp"
#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/dependency_graph/service_node.hpp"
#include "tt/parser/service/dependency_reader.hpp"

#include "tt/exception.hpp"

#include <iostream>

void tt::DependencyChecker::Validate(tt::DependencyGraph &graph,
                                     size_t starting_index) {
    auto services = graph.services();
    auto itr = services.begin();
    advance(itr, starting_index);
    for (; itr != services.end(); ++itr) {
        tt::DependencyReader dep_reader;
        std::visit(dep_reader, (*itr).service());
        const auto deps = dep_reader.dependencies();
        for (const auto &dep : deps) {
            std::cout << dep << std::endl;
            if (!graph.IsServiceActive(dep)) {
                throw tt::Exception("Dep not respected");
            }
        }
    }
}
