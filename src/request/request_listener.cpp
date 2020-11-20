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

#include "spdlog/spdlog.h" // for SPDLOG_WARN, SPDLOG_ERROR

#include "tl/expected.hpp" // for expected

#include "tt/net/server.hpp"                    // for Server
#include "tt/path/dirs.hpp"                     // for Dirs
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

    auto listen_err = server.Listen();
    // Wait until we cannot listen on the socket
    while (!listen_err) {
        SPDLOG_CRITICAL(listen_err.error());
        std::this_thread::sleep_for(std::chrono::seconds(1));
        listen_err = server.Listen();
    }

    for (;;) {
        auto buffer = server.ReceiveMessage();
        if (!buffer) {
            SPDLOG_ERROR(buffer.error());
            continue;
        }

        auto request = RequestFactory::GetRequestFromBuffer(buffer.value());
        if (!request) {
            SPDLOG_WARN(request.error());
            continue;
        }

        auto reply = request.value()->accept(dispatcher_);
        if (reply) {
            auto send_err = server.SendMessage(reply.value());
            if (!send_err) {
                SPDLOG_ERROR(send_err.error());
            }
        }
    }
}
