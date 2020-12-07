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

#include "tt/cli/command/show_command.hpp"

#include "nlohmann/json.hpp" // for json

#include "tl/expected.hpp" // for expected

#include "tt/cli/global_options.hpp"
#include "tt/log/cli_logger.hpp"                      // for CliLogger
#include "tt/net/client.hpp"                          // for Client
#include "tt/request/pack_request.hpp"                // for PackRequest
#include "tt/request/reply/adapter/service_info.hpp"  // IWYU pragma: keep
#include "tt/request/reply/get_reply_from_string.hpp" // for GetReplyFromString
#include "tt/request/service_info.hpp"                // for ServiceInfo
#include "tt/svc/live_service.hpp"                    // for LiveService
#include "tt/utils/deserialize.hpp"                   // for Deserialize

using nlohmann::json;

tt::cli::ShowCommand::ShowCommand(args::Subparser &parser)
    : Command(parser), service_(parser, "service", "service to show info of") {}

auto tt::cli::ShowCommand::Execute() -> int {
    if (!service_.HasPositional()) {
        logger()->LogCritical("{}", "Missing service");
        return 255;
    }

    net::Client client(dirs()->livedir() / "tt-ipc.socket");
    if (auto ret = client.Connect(); !ret.has_value()) {
        logger()->LogCritical("{}", ret.error());
        return 255;
    }

    request::ServiceInfo request(service_.Get());
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

    auto reply = request::GetReplyFromString(message_received.value());
    if (!reply.has_value()) {
        logger()->LogCritical("{}", reply.error());
        return 255;
    }

    try {
        auto live_service =
            reply.value().get<request::ServiceInfo::Reply>().Decode();
        if (!live_service.has_value()) {
            logger()->LogCritical("{}", live_service.error());
            return 255;
        }

        logger()->Print("name: {}\nstatus: {}\nremoval: {}",
                        live_service.value().name(),
                        GetServiceStatusName(live_service.value().status()),
                        live_service.value().IsMarkedForRemoval());

    } catch (nlohmann::detail::exception &e) {
        logger()->LogCritical("{}", e.what());
        return 255;
    }

    return 0;
}
