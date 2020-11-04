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

#include "tt/action/action_listener.hpp"

#include <cxxabi.h>     // for __forced_unwind
#include <future>       // for async, launch, launch::async
#include <memory>       // for allocator, unique_ptr
#include <string>       // for string
#include <system_error> // for system_error

#include "tt/action/action.hpp"         // for Action
#include "tt/action/action_factory.hpp" // for ActionFactory
#include "tt/net/server.hpp"            // for Server
#include "tt/net/socket.hpp"            // for Socket::Protocol, Socket::Pr...
#include "tt/path/dirs.hpp"             // for Dirs

void tt::ActionListener::Listen(std::shared_ptr<Dirs> dirs) {
    net::Server server{dirs->livedir() / "tt-ipc.socket"};

    for (;;) {
        auto buffer = server.ReceiveMessage();
        auto action_ptr = ActionFactory::GetActionFromBuffer(buffer);
        if (!action_ptr) {
            continue;
        }

        (void)std::async(std::launch::async, &Action::Apply, action_ptr.get());
    }
}
