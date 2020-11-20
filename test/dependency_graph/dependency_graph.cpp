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

        CHECK(graph.services().empty());
        CHECK(graph.enabled_services().empty());
    }

    SECTION("Add a service after removing one") {
        std::vector<std::string> filenames = {"../test/data/init-fsck",
                                              "../test/data/mount-rwfs"};
        REQUIRE_NOTHROW(tt::AddTestDependenciesToGraph(
            graph, {"init-fsck", "mount-rwfs"}, filenames));

        REQUIRE_NOTHROW(graph.RemoveServices({"init-fsck"}));

        REQUIRE_NOTHROW(tt::AddTestDependenciesToGraph(
            graph, {"udev"}, {"../test/data/udev.system"}));
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

    SECTION("Re-add an existing service") {
        const std::string service_name = "test-service";
        std::vector<std::string> filenames = {
            "../test/dependency_graph/version1/" + service_name};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {service_name}, filenames));

        REQUIRE_NOTHROW(tt::AddTestDependenciesToGraph(graph, {}, filenames));

        CHECK(graph.services().size() == 1);
    }

    SECTION("Replace a service") {
        const std::string service_name = "test-service";
        std::vector<std::string> filenames = {
            "../test/dependency_graph/version1/" + service_name};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {service_name}, filenames));

        filenames = {"../test/dependency_graph/version2/" + service_name};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {service_name}, filenames));

        // version2 has a stop script, while version1 does not
        CHECK(graph.services().size() == 1);
        CHECK(
            std::get<tt::Oneshot>(graph.GetServiceByName(service_name)).stop());
    }

    SECTION("Replace a service that has a dep on a newly added one") {
        const std::string service_name = "test-service";
        std::vector<std::string> filenames = {
            "../test/dependency_graph/version2/" + service_name};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {service_name}, filenames));

        // version3 has an additional depedency on dummy
        filenames = {"../test/dependency_graph/version3/" + service_name,
                     "../test/dependency_graph/dummy"};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {service_name}, filenames));

        CHECK(graph.services().size() == 2);
    }

    SECTION("Remove and add a service") {
        const std::string service_name = "test-service";
        const std::string dummy_path = "../test/dependency_graph/dummy";
        std::vector<std::string> filenames = {dummy_path};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {"dummy"}, filenames));
        REQUIRE(graph.services().size() == 1);

        filenames = {"../test/dependency_graph/version3/" + service_name};
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {service_name}, filenames));
        REQUIRE(graph.services().size() == 2);

        REQUIRE_NOTHROW(graph.RemoveServices({"dummy"}));
        REQUIRE(graph.services().size() == 2);
        REQUIRE_NOTHROW(graph.RemoveServices({service_name}));
        REQUIRE(graph.services().empty());

        filenames.push_back(dummy_path);
        REQUIRE_NOTHROW(
            tt::AddTestDependenciesToGraph(graph, {service_name}, filenames));
        REQUIRE(graph.services().size() == 2);
    }
}
