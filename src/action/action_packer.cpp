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

#include "tt/action/action_packer.hpp"

#include "msgpack.hpp"

#include "nngpp/buffer.h"

#include "tt/action/adapter/notify_up_action_adapter.hpp"
#include "tt/action/notify_up_action.hpp"

auto tt::ActionPacker::PackCommand(const char *command) -> std::stringstream {
    std::stringstream stream;
    msgpack::pack(stream, command);
    return stream;
}

auto tt::ActionPacker::Pack(const tt::NotifyUpAction &action) -> nng::buffer {
    auto stream = PackCommand("notify_up");

    msgpack::pack(stream, action);
    nng::buffer buffer{stream.str().c_str(), stream.str().size()};
    return buffer;
}
