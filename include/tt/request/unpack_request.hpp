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

#include <string>

#include "msgpack.hpp"

#include "tt/request/adapter/notify_up_request_adapter.hpp"

namespace tt {

template <typename T> auto UnpackRequest(const std::string &buffer) -> T {
    static_assert(std::is_base_of_v<Request, T>, "T must derive from Request");
    msgpack::object_handle result;
    msgpack::unpack(result, buffer.c_str(), buffer.size());
    msgpack::object obj(result.get());

    return obj.as<T>();
}

} // namespace tt
