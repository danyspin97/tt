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

#include "tt/cli/command/start_command.hpp"

#include "tt/cli/global_options.hpp"
#include "tt/log/cli_logger.hpp"       // for CliLogger
#include "tt/request/make_request.hpp" // for MakeRequest

tt::cli::StartCommand::StartCommand(args::Subparser &parser)
    : Command(parser), services_(parser, "service", "services to start") {}

auto tt::cli::StartCommand::Execute() -> int {
    net::Client client(dirs()->livedir() / "tt-ipc.socket");
    if (auto ret = client.Connect(); !ret.has_value()) {
        logger()->LogError("{}", ret.error());
        return 255;
    }

    auto services = std::move(args::get(services_));
    if (services.empty()) {
        logger()->LogError("No service passed");
        return 255;
    }

    auto reply = request::MakeRequest(
        dirs(), request::StartServices{std::move(services)});
    if (!reply.has_value()) {
        logger()->LogError("{}", reply.error());
    }

    return 0;
}
