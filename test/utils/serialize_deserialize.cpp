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

#include "tt/utils/serialize.hpp"
#include "tt/utils/deserialize.hpp"

#include "catch2/catch.hpp"

#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/dependency_graph/utils.hpp"
#include "tt/utils/read_buffer_from_file.hpp"

TEST_CASE("Serialize and Deserialize") {
    tt::DependencyGraph graph;
    std::vector<std::string> filenames = {"../test/data/mount-fstab",
                                          "../test/data/init-fsck",
                                          "../test/data/mount-rwfs"};
    tt::AddTestDependenciesToGraph(graph, {"mount-fstab"}, filenames);

    const auto *graph_test_file = "test_serialize_graph";
    if (std::filesystem::exists(graph_test_file)) {
        std::filesystem::remove(graph_test_file);
    }

    tt::utils::Serialize(graph, graph_test_file);
    REQUIRE(std::filesystem::exists(graph_test_file));

    auto buffer = tt::utils::ReadBufferFromFile(graph_test_file);
    auto deserialized_graph =
        tt::utils::Deserialize<tt::DependencyGraph>(std::move(buffer));

    CHECK(graph.enabled_services() == deserialized_graph.enabled_services());
    CHECK(graph.nodes() == deserialized_graph.nodes());
}
