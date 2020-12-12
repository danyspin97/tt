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
#include "tt/dependency_graph/utils.hpp" // for GetName
#include "tt/file_lock.hpp"              // for FileLock
#include "tt/log/cli_logger.hpp"         // for CliLogger
#include "tt/parser/parser_error.hpp"    // for ParserError
#include "tt/parser/service/services_parser.hpp"
#include "tt/path/dirs.hpp"
#include "tt/request/make_request.hpp" // for MakeRequest
#include "tt/utils/deserialize.hpp"
#include "tt/utils/serialize.hpp"

tt::cli::EnableCommand::EnableCommand(args::Subparser &parser)
    : Command(parser), service_list_(parser, "services", "services to enable"),
      update_services_(parser, "update",
                       "update services if they are already enabled",
                       {'u', "update"}),
      no_update_dependencies_(parser, "no dependencies update",
                              "don't update dependencies, only add new ones",
                              {"dont-update-dependencies"}) {}

auto tt::cli::EnableCommand::Execute() -> int {
    if (service_list_.cbegin() == service_list_.cend()) {
        throw tt::Exception("At least one service must be provided");
    }

    FileLock lock(dirs()->livedir() / ".graph_lock");
    if (!lock.TryLock()) {
        throw Exception(
            "Another instance of tt-enable or tt-disable is running");
    }

    return EnableServices();
}

auto tt::cli::EnableCommand::EnableServices() -> int {
    auto &service_list = args::get(service_list_);

    DependencyGraph graph;
    auto graph_filename = GetGraphFilename(dirs());
    if (std::filesystem::exists(graph_filename)) {
        graph = utils::Deserialize<DependencyGraph>(graph_filename);
    }
    // If the graph is new, all the following checks are useless
    // However, since the graph is empty, they will be fast to execute and
    // most of the time we are enabling services to an existing
    // graph anyway

    if (!update_services_) {
        for (const auto &service : service_list) {
            if (graph.IsServiceEnabled(service)) {
                throw Exception(
                    fmt::format("service {} is already enabled", service));
            }
        }
    }

    ServicesParser parser(dirs());
    auto ret = parser.ParseServices(service_list);
    if (!ret.has_value()) {
        logger()->LogError("{}", ret.error().msg());
        return 1;
    }

    auto services = ret.value();
    if (no_update_dependencies_) {
        services.erase(std::remove_if(services.begin(), services.end(),
                                      [graph](const auto &service) {
                                          return graph.HasService(
                                              std::visit(GetName, service));
                                      }),
                       services.end());
    }

    graph.AddServices(service_list, services);
    utils::Serialize(graph, graph_filename);

    if (!std::filesystem::exists(dirs()->livedir() / ".svc_lock")) {
        return 0;
    }

    net::Client client(dirs()->livedir() / "tt-ipc.socket");
    if (auto ret = client.Connect(); !ret.has_value()) {
        logger()->LogCritical("{}", ret.error());
        return 255;
    }

    auto reply = request::MakeRequest(dirs(), request::ReloadGraph{});
    if (!reply.has_value()) {
        logger()->LogError("{}", reply.error());
    }

    return 0;
}
