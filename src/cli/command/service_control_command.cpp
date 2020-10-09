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
#include "tt/cli/utils.hpp"
#include "tt/dependency_graph/dependency_graph_serializer.hpp"
#include "tt/dependency_graph/dependency_reader.hpp"
#include "tt/svc/service_status_manager.hpp"
#include "tt/svc/spawn_service.hpp"
#include "tt/user_dirs.hpp"

tt::cli::ServiceControlCommand::ServiceControlCommand(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    : Command(parser, std::move(common_options)) {}

auto tt::cli::ServiceControlCommand::Execute() -> int {
    if (geteuid() > 0) {
        return StartUserServices();
    }

    return StartSystemServices();
}

auto tt::cli::ServiceControlCommand::StartUserServices() -> int {
    auto &user_dirs = UserDirs::GetInstance();
    // auto &dirs = Dirs::GetInstance();

    // Start action listener
    auto graph = ReadGraphFromFile(user_dirs.statedir() / "graph");
    auto services = graph.GetActiveServices();
    ServiceStatusManager::GetInstance().Initialize(services);

    auto action_listener = std::thread(&ActionListener::Listen);

    // TODO: Calculate an optimal order of services to start
    auto nodes = graph.nodes();
    for (auto &node : nodes) {
        // https://stackoverflow.com/questions/23454793/whats-the-c-11-way-to-fire-off-an-asynchronous-task-and-forget-about-it
        auto futptr = std::make_shared<std::future<void>>();
        *futptr = std::async(std::launch::async,
                             [*this, futptr, node]() { SpawnNode(node); });
    }

    // Runs indefinitely
    action_listener.join();

    return 0;
}

auto tt::cli::ServiceControlCommand::StartSystemServices() -> int {
    auto &dirs = Dirs::GetInstance();
    auto graph = ReadGraphFromFile(dirs.statedir() / "graph");
    return 0;
}

void tt::cli::ServiceControlCommand::SpawnNode(const ServiceNode &node) {
    DependencyReader dep_reader{};
    std::visit(dep_reader, node.service());
    auto deps = dep_reader.dependencies();

    auto &manager = ServiceStatusManager::GetInstance();
    for (auto &dep : deps) {
        manager.WaitOnService(dep);
    }

    SpawnService(node.service());
}

auto tt::cli::ServiceControlCommand::ReadGraphFromFile(
    std::filesystem::path &&graph_path) -> tt::DependencyGraph {
    auto buffer = ReadBufferFromFile(std::move(graph_path));
    return DependencyGraphSerializer::Deserialize(buffer, buffer.size());
}
