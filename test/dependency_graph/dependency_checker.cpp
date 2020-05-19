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

#include "catch2/catch.hpp"

#include "tt/data/service.hpp"
#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/dependency_graph/service_node.hpp"
#include "tt/parser/service/service_parser.hpp"
#include "tt/parser/service/services_parser.hpp"

TEST_CASE("DependencyChecker") {
    tt::DependencyGraph graph;

    SECTION("mount-fstab with unresolved deps") {
        auto parser = tt::ServiceParser("../test/data/mount-fstab");
        auto service = parser.service();
        auto services = std::vector<tt::Service>{service};
        graph.AddNodes(services);
        REQUIRE_THROWS(tt::DependencyChecker::Validate(graph, 0));
    }

    SECTION("mount-fstab with unresolved deps") {
        auto files = {"../test/data/mount-fstab", "../test/data/init-fsck",
                      "../test/data/mount-rwfs"};
        auto services = std::vector<tt::Service>{};
        for (const auto &file : files) {
            auto parser = tt::ServiceParser(file);
            auto service = parser.service();
            services.push_back(service);
        }
        graph.AddNodes(services);
        REQUIRE_THROWS(tt::DependencyChecker::Validate(graph, 0));
    }
}
