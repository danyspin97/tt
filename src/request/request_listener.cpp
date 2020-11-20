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

#include "tt/request/request_listener.hpp"

#include <cxxabi.h>     // for __forced_unwind
#include <future>       // for async, launch, launch::async
#include <memory>       // for allocator, unique_ptr
#include <string>       // for string
#include <system_error> // for system_error

#include "tt/net/server.hpp" // for Server
#include "tt/net/socket.hpp" // for Socket::Protocol, Socket::Pr...
#include "tt/path/dirs.hpp"  // for Dirs
#include "tt/request/notify_service_status.hpp" // for NotifyServiceStatus
#include "tt/request/request.hpp"               // for Action
#include "tt/request/request_factory.hpp"       // for ActionFactory
#include "tt/svc/service_manager.hpp"           // for ServiceManager
#include "tt/utils/launch_async.hpp"            // for LaunchAsync

tt::request::RequestListener::RequestListener(ServiceManager &service_manager,
                                              const std::shared_ptr<Dirs> &dirs)
    : dispatcher_(service_manager),
      socket_path_(dirs->livedir() / "tt-ipc.socket") {}

void tt::request::RequestListener::Listen() {
    net::Server server{socket_path_};

    // Wait until we cannot listen on the socket
    while (!server.Listen()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (;;) {
        auto buffer = server.ReceiveMessage();
        auto request = RequestFactory::GetRequestFromBuffer(buffer);

        tt::LaunchAsync([this, &request]() { request->accept(dispatcher_); });
    }
}
