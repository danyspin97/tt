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

#include "nngpp/protocol/rep0.h" // for rep::open

#include "tt/exception.hpp" // for Exception

tt::net::Server::Server(Protocol protocol, std::string address, uint16_t port)
    : Socket(nng::rep::open(), protocol, std::move(address), port) {}

void tt::net::Server::Listen() {
    if (is_listening) {
        return;
    }

    socket().listen(GetNetAddress().c_str());
    is_listening = true;
}

auto tt::net::Server::ReceiveMessage() -> std::string {
    if (!is_listening) {
        throw tt::Exception("Must listen on socket first");
    }

    auto buffer = socket().recv();
    return std::string{static_cast<const char *>(buffer.data()), buffer.size()};
}
