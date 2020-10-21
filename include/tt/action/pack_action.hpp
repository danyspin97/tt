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

#include <sstream>

// Include our adapter of NotifyUpAction
#include "tt/action/adapter/notify_up_action_adapter.hpp"

#include "msgpack.hpp"

#include "nngpp/buffer.h"

#include "tt/action/notify_up_action.hpp"

namespace tt {

auto PackAction(const Action &action) -> nng::buffer;

template <typename T> auto PackAction(const T &action) -> nng::buffer {
    static_assert(std::is_base_of<Action, T>::value,
                  "T must derive from Action");
    std::stringstream stream;
    msgpack::pack(stream, action);
    return nng::buffer{stream.str().c_str(), stream.str().size()};
}

} // namespace tt
