
#include "tt/dependency_graph/utils.hpp"

#include "tt/data/service.hpp"
#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/parser/service/service_parser.hpp"

void tt::AddTestDependenciesToGraph(tt::DependencyGraph &graph,
                                    const std::vector<std::string> &filenames) {
    auto services = std::vector<tt::Service>{};
    for (const auto &file : filenames) {
        auto parser = tt::ServiceParser(file);
        auto service = parser.service();
        services.push_back(service);
    }
    graph.AddNodes(services);
}
