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

#include "tt/dependency_graph/dependency_graph.hpp"

#include "catch2/catch.hpp" // for operator""_catch_sr, Source...

#include "tt/dependency_graph/utils.hpp" // for AddTestDependenciesToGraph
#include "tt/exception.hpp"              // IWYU pragma: keep

TEST_CASE("DependencyChecker") {
    tt::DependencyGraph graph;

    SECTION("Add mount-fstab with resolved deps and remove it") {
        std::vector<std::string> filenames = {"../test/data/mount-fstab",
                                              "../test/data/init-fsck",
                                              "../test/data/mount-rwfs"};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {"mount-fstab"}, filenames));

        REQUIRE_NOTHROW(graph.RemoveServices({"mount-fstab"}));

        CHECK(graph.nodes().empty());
        CHECK(graph.enabled_services().empty());
    }

    SECTION("mount-fstab with unresolved deps") {
        std::vector<std::string> filenames = {"../test/data/mount-fstab"};
        REQUIRE_THROWS_AS(
            tt::AddTestDependenciesToGraph(graph, {"mount-fstab"}, filenames),
            tt::Exception);
    }

    SECTION("CheckGraphIsAcyclic") {
        std::vector<std::string> filenames = {"../test/data/selfdep"};
        REQUIRE_THROWS_AS(
            tt::AddTestDependenciesToGraph(graph, {"selfdep"}, filenames),
            tt::Exception);
    }

    SECTION("CheckGraphIsAcyclic with self dependency") {
        std::vector<std::string> filenames = {
            "../test/data/foo", "../test/data/bar", "../test/data/foobar"};
        REQUIRE_THROWS_AS(
            tt::AddTestDependenciesToGraph(graph, {"foobar"}, filenames),
            tt::Exception);
    }
}
