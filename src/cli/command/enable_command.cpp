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

#include "tt/cli/command/enable_command.hpp"

#include <unistd.h>

#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/dependency_graph/get_graph_filename.hpp"
#include "tt/file_lock.hpp" // for FileLock
#include "tt/parser/service/services_parser.hpp"
#include "tt/path/dirs.hpp"
#include "tt/utils/deserialize.hpp"
#include "tt/utils/serialize.hpp"

tt::cli::EnableCommand::EnableCommand(args::Subparser &parser)
    : Command(parser), service_list_(parser, "services", "services to parse") {}

auto tt::cli::EnableCommand::Execute() -> int { return EnableServices(); }

auto tt::cli::EnableCommand::EnableServices() -> int {
    FileLock lock(dirs()->supervisedir() / ".graph_lock");
    if (!lock.TryLock()) {
        throw Exception(
            "Another instance of tt-enable or tt-disable is running");
    }

    const auto &services_list = args::get(service_list_);
    if (services_list.empty()) {
        throw tt::Exception("At least one service must be provided");
    }

    ServicesParser parser(dirs());
    parser.ParseServices(services_list);
    const std::vector<tt::Service> services = parser.services();

    DependencyGraph graph;
    auto graph_filename = GetGraphFilename(dirs());
    if (std::filesystem::exists(graph_filename)) {
        graph = utils::Deserialize<DependencyGraph>(graph_filename);
    }

    // TODO: Check if the service is already enabled
    // and if it is different
    graph.AddServices(services_list, services);

    utils::Serialize(graph, graph_filename);
    return 0;
}
