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

#include <sstream> // for stringstream
#include <string>  // for string

#include "fmt/format.h"      // for format
#include "nlohmann/json.hpp" // for json
#include "tl/expected.hpp"   // for expected

#include "tt/request/reply/adapter/service_info.hpp" // IWYU pragma: keep
#include "tt/request/reply/adapter/system_info.hpp"  // IWYU pragma: keep

namespace tt::request {

class Request;

template <typename T>
auto PackReply(const T &reply) -> tl::expected<nlohmann::json, std::string> {
    static_assert(std::is_base_of_v<Reply, T>, "T must derive from Reply");
    try {
        return reply;
    } catch (nlohmann::detail::exception &e) {
        return tl::make_unexpected(
            fmt::format("'{}', while converting reply to JSON", e.what()));
    }
}

} // namespace tt::request
