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

#include "tt/cli/command/disable_command.hpp"

#include "args.hxx"

#include "tt/cli/global_options.hpp"                  // for GlobalOptions
#include "tt/dependency_graph/dependency_graph.hpp"   // for DependencyGraph
#include "tt/dependency_graph/get_graph_filename.hpp" // for GetGraphFilename
#include "tt/exception.hpp"                           // for Exception
#include "tt/file_lock.hpp"                           // for FileLock
#include "tt/log/cli_logger.hpp"                      // for CliLogger
#include "tt/net/client.hpp"                          // for Client
#include "tt/path/dirs.hpp"                           // for Dirs
#include "tt/request/pack_request.hpp"                // for PackRequest
#include "tt/utils/deserialize.hpp"                   // for Deserialize
#include "tt/utils/serialize.hpp"                     // for Serialize

tt::cli::DisableCommand::DisableCommand(args::Subparser &parser)
    : Command(parser), services_(parser, "service", "services to disable") {}

auto tt::cli::DisableCommand::Execute() -> int {
    if (services_.cbegin() == services_.cend()) {
        throw tt::Exception("At least one service must be provided");
    }

    FileLock lock(dirs()->livedir() / ".graph_lock");
    if (!lock.TryLock()) {
        throw Exception(
            "Another instance of tt-enable or tt-disable is running");
    }

    return DisableServices();
}

auto tt::cli::DisableCommand::DisableServices() -> int {
    auto &services = args::get(services_);

    DependencyGraph graph;
    auto graph_filename = GetGraphFilename(dirs());
    if (!std::filesystem::exists(graph_filename)) {
        throw Exception("No service has been enabled yet");
    }
    graph = utils::Deserialize<DependencyGraph>(graph_filename);

    graph.RemoveServices(services);
    utils::Serialize(graph, graph_filename);

    if (!std::filesystem::exists(dirs()->livedir() / ".svc_lock")) {
        return 0;
    }

    net::Client client(dirs()->livedir() / "tt-ipc.socket");
    if (auto ret = client.Connect(); !ret.has_value()) {
        logger()->LogCritical("{}", ret.error());
        return 255;
    }

    request::ReloadGraph request;
    auto s = request::PackRequest(request);
    if (auto ret = client.SendMessage(s); !ret.has_value()) {
        logger()->LogCritical("{}", ret.error());
        return 255;
    }
    auto message_received = client.ReceiveMessage();
    if (!message_received.has_value()) {
        logger()->LogCritical("{}", message_received.error());
        return 255;
    }

    return 0;
}
