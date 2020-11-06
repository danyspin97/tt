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

#include <nlohmann/json.hpp>

#include "tt/request/adapter/notify_service_statup.hpp" // IWYU pragma: keep

namespace tt::request {

class Request;

template <typename T> auto PackRequest(const T &request) -> std::string {
    static_assert(std::is_base_of_v<Request, T>, "T must derive from Request");
    nlohmann::json j;
    j["request_name"] = T::name;
    j["request"] = request;
    return j.dump();
}

} // namespace tt::request
