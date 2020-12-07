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

#include "tt/request/reply/get_reply_from_string.hpp"

#include "fmt/format.h" // for format

#include "nlohmann/json.hpp" // for json

#include "tl/expected.hpp" // for expected

using nlohmann::json;

auto tt::request::GetReplyFromString(std::string reply)
    -> tl::expected<json, std::string> {
    try {
        auto j = json::parse(reply);
        if (!j.at("ok").get<bool>()) {
            return tl::make_unexpected(j.at("error").get<std::string>());
        }
        return j.at("reply");
    } catch (const nlohmann::detail::exception &e) {
        return tl::make_unexpected(
            fmt::format("'{}', while parsing reply '{}'", e.what(), reply));
    }
}
