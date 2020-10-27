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

#include "tt/net/socket.hpp"

#include "fmt/format.h" // for format

tt::net::Socket::Socket(nng::socket &&socket, Protocol protocol,
                        std::string address, uint16_t port)
    : socket_(std::move(socket)), protocol_(protocol),
      address_(std::move(address)), port_(port) {}

auto tt::net::Socket::GetNetAddress() const -> std::string {
    const char *protocol_str = protocol_ == Protocol::UDP ? "udp" : "tcp";
    return fmt::format("{}://{}:{}", protocol_str, address_,
                       std::to_string(port_));
}
auto tt::net::Socket::socket() const -> nng::socket_view { return socket_; }
