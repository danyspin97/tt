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
#include <string>       // for operator+, string
#include <system_error> // for system_error

#include "nngpp/buffer.h"        // for buffer
#include "nngpp/error.h"         // for exception
#include "nngpp/protocol/rep0.h" // for open
#include "nngpp/socket.h"        // for socket

#include "tt/action/action.hpp"         // for Action
#include "tt/action/action_factory.hpp" // for ActionFactory
#include "tt/exception.hpp"             // for Exception

void tt::ActionListener::Listen() try {
    nng::socket socket = nng::rep::open();
    socket.listen("tcp://localhost:8000");

    for (;;) {
        nng::buffer buffer = socket.recv();
        auto action_ptr = ActionFactory::GetActionFromBuffer(buffer);
        if (!action_ptr) {
            continue;
        }

        (void)std::async(std::launch::async, &Action::Apply, action_ptr.get());
    }
} catch (const nng::exception &e) {
    throw tt::Exception(
        std::string{
            "Couldn't listen on nng socket due to the following error: "} +
        std::string{e.what()});
}
