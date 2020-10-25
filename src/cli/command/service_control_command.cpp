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

#include "tt/cli/command/service_control_command.hpp"

#include <unistd.h>

#include <future>

#include "tt/action/action_listener.hpp"
#include "tt/dependency_graph/dependency_reader.hpp"
#include "tt/dependency_graph/get_graph_filename.hpp"
#include "tt/exception.hpp"
#include "tt/supervision/service_status_manager.hpp"
#include "tt/supervision/supervise_service.hpp"

tt::cli::ServiceControlCommand::ServiceControlCommand(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    : Command(parser, std::move(common_options)) {}

auto tt::cli::ServiceControlCommand::Execute() -> int {
    return StartServices();
}

auto tt::cli::ServiceControlCommand::StartServices() -> int {
    auto graph_filename = GetGraphFilename(dirs());
    if (!std::filesystem::exists(graph_filename) ||
        !std::filesystem::is_regular_file(graph_filename)) {
        throw Exception("Could not find the serialized graph.");
    }
    auto graph = utils::Deserialize<DependencyGraph>(graph_filename);
    auto services = graph.GetActiveServices();
    ServiceStatusManager::GetInstance().Initialize(services);

    // Start action listener
    auto action_listener = std::thread(&ActionListener::Listen);

    // TODO: Calculate an optimal order of services to start
    auto nodes = graph.nodes();
    for (const auto &node : nodes) {
        // https://stackoverflow.com/questions/23454793/whats-the-c-11-way-to-fire-off-an-asynchronous-task-and-forget-about-it
        auto futptr = std::make_shared<std::future<void>>();
        *futptr = std::async(std::launch::async,
                             [this, futptr, node]() { SpawnNode(node); });
    }

    // Runs indefinitely
    action_listener.join();

    return 0;
}

void tt::cli::ServiceControlCommand::SpawnNode(const ServiceNode &node) {
    DependencyReader dep_reader{};
    std::visit(dep_reader, node.service());
    auto deps = dep_reader.dependencies();

    const auto &manager = ServiceStatusManager::GetInstance();
    auto deps_satisfied = true;
    for (const auto &dep : deps) {
        if (!manager.WaitOnServiceStart(dep)) {
            deps_satisfied = false;
            break;
        }
    }

    if (!deps_satisfied) {
        manager.ServiceStartUpdate(node.name(), false);
        return;
    }
    SuperviseService supervise{dirs()};
    std::visit(supervise, node.service());
}
