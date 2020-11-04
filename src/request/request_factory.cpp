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

#include "tt/request/request_factory.hpp"

#include <string> // for string, operator==

#include "msgpack.hpp" // IWYU pragma: keep

#include "tt/exception.hpp"                                 // for Exception
#include "tt/request/adapter/notify_up_request_adapter.hpp" // IWYU pragma: keep
#include "tt/request/notify_up_request.hpp" // for NotifyUpRequest
#include "tt/request/request.hpp"           // for Request

auto tt::RequestFactory::GetRequestFromBuffer(const std::string &buffer)
    -> std::unique_ptr<Request> try {
    msgpack::object_handle result;
    msgpack::unpack(result, buffer.data(), buffer.size());
    msgpack::object obj(result.get());

    // ComplexRequest:
    //     Request:
    //         name_: "complex_action"
    //     data:       ...
    //     other_data: ...
    std::string action_name =
        obj.via.map.ptr[0].val.via.map.ptr[0].val.as<std::string>();

    std::unique_ptr<Request> action_ptr = nullptr;
    if (action_name == "notify_up") {
        action_ptr = std::make_unique<NotifyUpRequest>(obj.as<NotifyUpRequest>());
    }
    return action_ptr;
} catch (const msgpack::type_error & /*e*/) {
    throw tt::Exception(
        "There was an error while parsing the buffer containing the action");
}
