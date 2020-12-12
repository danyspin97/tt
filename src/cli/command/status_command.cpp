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

#include "tt/cli/command/status_command.hpp"

#include "tl/expected.hpp" // for expected

#include "tt/cli/global_options.hpp"
#include "tt/log/cli_logger.hpp"                      // for CliLogger
#include "tt/request/make_request.hpp"                // for MakeRequest
#include "tt/request/reply/adapter/system_info.hpp"   // IWYU pragma: include
#include "tt/request/reply/get_reply_from_string.hpp" // for GetReplyFromString
#include "tt/request/reply/system_info.hpp"           // for SystemInfo

tt::cli::StatusCommand::StatusCommand(args::Subparser &parser)
    : Command(parser) {}

auto tt::cli::StatusCommand::Execute() -> int {
    auto reply = request::MakeRequest(dirs(), request::SystemInfo{});
    if (!reply.has_value()) {
        logger()->LogError("{}", reply.error());
    }

    std::vector<LiveService> live_services;
    try {
        auto ret = reply.value().get<request::SystemInfo::Reply>().Decode();
        if (!ret.has_value()) {
            logger()->LogCritical("{}", ret.error());
            return 255;
        }

        live_services = std::move(ret.value());
    } catch (nlohmann::detail::exception &e) {
        logger()->LogCritical("{}", e.what());
        return 255;
    }

    for (auto &live_service : live_services) {
        logger()->Print("- {}\n    status: {}\n", live_service.name(),
                        GetServiceStatusName(live_service.status()));
    }

    return 0;
}
