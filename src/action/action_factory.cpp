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

#include "tt/action/action_factory.hpp"

#include "nngpp/nngpp.h"

#include "msgpack.hpp"

#include "tt/action/adapter/notify_up_action_adapter.hpp"
#include "tt/action/notify_up_action.hpp"

auto tt::ActionFactory::GetActionFromBuffer(const nng::buffer& buffer)
    -> std::unique_ptr<Action> {
    msgpack::unpacker pac;
    // Copy the buffer data to the unpacker object
    pac.reserve_buffer(buffer.size());
    memcpy(pac.buffer(), buffer.data(), buffer.size());
    pac.buffer_consumed(buffer.size());

    msgpack::unpacked msg;
    pac.next(msg);
    msgpack::object obj = msg.get();
    std::string action_name;
    obj.convert(action_name);

    pac.next(msg);
    obj = msg.get();
    std::unique_ptr<Action> action_ptr;
    if (action_name == "notify_up") {
        auto action = obj.as<NotifyUpAction>();
        action_ptr = std::make_unique<NotifyUpAction>(action);
    }
    return action_ptr;
}
