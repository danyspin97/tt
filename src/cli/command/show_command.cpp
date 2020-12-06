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

#include "Base64.h"

#include "tl/expected.hpp" // for expected

#include "tt/cli/global_options.hpp"
#include "tt/log/cli_logger.hpp"                 // for CliLogger
#include "tt/net/client.hpp"                     // for Client
#include "tt/request/pack_request.hpp"           // for PackRequest
#include "tt/request/service_status_request.hpp" // for ServiceStatusRequest
#include "tt/svc/live_service.hpp"               // for LiveService
#include "tt/utils/deserialize.hpp"              // for Deserialize

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

    request::ServiceStatusRequest request(service_.Get());
    auto s = request::PackRequest(request);
    if (auto ret = client.SendMessage(s); !ret.has_value()) {
        logger()->LogCritical("{}", ret.error());
        return 255;
    }
    auto reply = client.ReceiveMessage();
    if (!reply.has_value()) {
        logger()->LogCritical("{}", reply.error());
        return 255;
    }

    auto j = json::parse(reply.value());
    if (!j["ok"].get<bool>()) {
        logger()->LogCritical("{}", j["error"].get<std::string>());
        return 255;
    }
    auto base64_status = j["data"].get<std::string>();
    std::string encoded_status;
    macaron::Base64::Decode(base64_status, encoded_status);
    std::vector<uint8_t> buffer;
    buffer.reserve(encoded_status.size());
    std::copy(encoded_status.begin(), encoded_status.end(),
              std::back_insert_iterator(buffer));
    LiveService live_service;
    auto state = bitsery::quickDeserialization<
        bitsery::InputBufferAdapter<std::vector<uint8_t>>>(
        {buffer.begin(), buffer.size()}, live_service);
    if (state.first == bitsery::ReaderError::NoError && state.second) {
        logger()->LogCritical("{}", state.second);
        return 255;
    }

    logger()->Print("name: {}\nstatus: {}\nremoval: {}", live_service.name(),
                    GetServiceStatusName(live_service.status()),
                    live_service.IsMarkedForRemoval());

    return 0;
}
