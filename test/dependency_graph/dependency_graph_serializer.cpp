
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
    tt::AddTestDependenciesToGraph(graph, filenames);
    std::vector<std::string> services_to_enable{"mount-fstab"};
    graph.AddEnabledServices(services_to_enable);

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
