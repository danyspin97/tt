
#include "tt/dependency_graph/dependency_graph_serializer.hpp"

#include "catch2/catch.hpp"

#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/dependency_graph/utils.hpp"
#include "tt/dirs.hpp"

TEST_CASE("DependencyGraphSerializer") {
    tt::DependencyGraphSerializer::Buffer buffer;
    tt::DependencyGraph graph;
    std::vector<std::string> filenames = {"../test/data/mount-fstab",
                                          "../test/data/init-fsck",
                                          "../test/data/mount-rwfs"};
    tt::AddTestDependenciesToGraph(graph, filenames);

    SECTION("Serialize and deserialize correctly") {
        auto result = tt::DependencyGraphSerializer::Serialize(graph);
        buffer = std::get<0>(result);
        auto writtenSize = std::get<1>(result);

        tt::DependencyGraph deserialized_graph =
            tt::DependencyGraphSerializer::Deserialize(buffer, writtenSize);

        CHECK(graph.enabled_services() ==
              deserialized_graph.enabled_services());
        CHECK(graph.nodes() == deserialized_graph.nodes());
    }
}
