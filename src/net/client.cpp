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

#include "tt/net/client.hpp"

#include "nngpp/protocol/req0.h" // for req::open

#include "tt/exception.hpp" // for Exception

tt::net::Client::Client(Protocol protocol, std::string address, uint16_t port)
    : Socket(nng::req::open(), protocol, std::move(address), port) {}

void tt::net::Client::Connect() {
    if (is_connected) {
        return;
    }

    socket().dial(GetNetAddress().c_str());
    is_connected = true;
}

void tt::net::Client::SendMessage(const std::string &message) {
    if (!is_connected) {
        throw tt::Exception("Must connect first");
    }

    nng::buffer msg = nng::make_buffer(message.size());
    memcpy(msg.data(), message.data(), message.size());
    socket().send(msg);
}
