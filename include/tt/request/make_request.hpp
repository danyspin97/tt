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

#pragma once

#include "tt/net/client.hpp"                          // for Client
#include "tt/path/dirs.hpp"                           // for Dirs
#include "tt/request/pack_request.hpp"                // for PackRequest
#include "tt/request/reply/get_reply_from_string.hpp" // for GetReplyFromString

namespace tt::request {

template <typename T>
[[nodiscard]] auto MakeRequest(const std::shared_ptr<Dirs> &dirs, T request)
    -> tl::expected<nlohmann::json, std::string> {
    net::Client client(dirs->livedir() / "tt-ipc.socket");
    if (auto ret = client.Connect(); !ret.has_value()) {
        return tl::make_unexpected(std::move(ret.error()));
    }

    auto s = request::PackRequest(request);
    if (auto ret = client.SendMessage(s); !ret.has_value()) {
        return tl::make_unexpected(std::move(ret.error()));
    }

    auto ret = client.ReceiveMessage().and_then(request::GetReplyFromString);
    if (!ret.has_value()) {
        return tl::make_unexpected(std::move(ret.error()));
    }

    return std::move(ret.value());
}

} // namespace tt::request
