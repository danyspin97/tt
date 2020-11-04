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

#include <map>

#include "msgpack.hpp"

#include "tt/request/notify_up_request.hpp"

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
    namespace adaptor {

    // Place class template specialization here
    template <> struct as<tt::NotifyUpRequest> {
        auto operator()(msgpack::object const &o) const -> tt::NotifyUpRequest {
            if (o.type != msgpack::type::MAP) {
                throw msgpack::type_error();
            }
            if (o.via.map.size != 3) {
                throw msgpack::type_error();
            }
            if (o.via.map.ptr[0].val.via.map.ptr[0].val.as<std::string>() !=
                "notify_up") {
                throw msgpack::type_error();
            }

            auto service = o.via.map.ptr[1].val.as<std::string>();
            auto succeded = o.via.map.ptr[2].val.as<bool>();
            return tt::NotifyUpRequest{std::move(service), succeded};
        }
    };

    } // namespace adaptor
}
} // namespace msgpack
