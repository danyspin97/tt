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

#include "tt/dependency_graph/dependency_graph_serializer.hpp"

#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/dependency_graph/utils.hpp"
#include "tt/dirs.hpp"

#include "catch2/catch.hpp"

TEST_CASE("DependencyGraphSerializer") {
    tt::DependencyGraphSerializer::Buffer buffer;
    tt::DependencyGraph graph;
    std::vector<std::string> filenames = {"../test/data/mount-fstab",
                                          "../test/data/init-fsck",
                                          "../test/data/mount-rwfs"};
    tt::AddTestDependenciesToGraph(graph, {"mount-fstab"}, filenames);

    SECTION("Serialize and deserialize "
            "correctly") {
        auto serialized_data = tt::DependencyGraphSerializer::Serialize(graph);

        tt::DependencyGraphSerializer::Buffer buffer =
            std::get<0>(serialized_data);
        size_t written_size = std::get<1>(serialized_data);

        tt::DependencyGraph deserialized_graph =
            tt::DependencyGraphSerializer::Deserialize(buffer, written_size);

        CHECK(graph.enabled_services() ==
              deserialized_graph.enabled_services());
        CHECK(graph.nodes() == deserialized_graph.nodes());
    }
}
