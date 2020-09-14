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

#include "tt/dependency_graph/utils.hpp"

#include "tt/data/service.hpp"
#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/parser/service/service_parser.hpp"

void tt::AddTestDependenciesToGraph(
    tt::DependencyGraph &graph,
    const std::vector<std::string> &services_to_enable,
    const std::vector<std::string> &filenames) {
    auto services = std::vector<tt::Service>{};
    for (const auto &file : filenames) {
        auto parser = tt::ServiceParser(file);
        auto service = parser.service();
        services.push_back(service);
    }
    graph.AddServices(services_to_enable, services);
}
