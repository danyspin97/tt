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

#include "tt/net/server.hpp"

#include <utility> // for move

#include "nngpp/protocol/rep0.h" // for open
#include "nngpp/socket_view.h"   // for socket_view

#include "tt/exception.hpp" // for Exception

tt::net::Server::Server(std::filesystem::path socket_path)
    : Socket(nng::rep::open(), std::move(socket_path)) {}

auto tt::net::Server::Listen() -> bool {
    if (is_listening_) {
        return true;
    }
    try {
        socket().listen(GetNetAddress().c_str());
        is_listening_ = true;
    } catch (nng::exception & /*e*/) {
        return false;
    }
    return true;
}

auto tt::net::Server::IsListening() const noexcept -> bool {
    return is_listening_;
}
