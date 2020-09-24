/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include "msgpack.hpp"

#include "tt/action/notify_up_action.hpp"

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
    namespace adaptor {

    // Place class template specialization here
    template <> struct as<tt::NotifyUpAction> {
        tt::NotifyUpAction operator()(msgpack::object const &o) const {
            if (o.type != msgpack::type::MAP) {
                throw msgpack::type_error();
            }
            if (o.via.array.size != 2) {
                throw msgpack::type_error();
            }
            auto service = o.as<std::string>();
            return tt::NotifyUpAction{std::move(service)};
        }
    };

    } // namespace adaptor
}
} // namespace msgpack
