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

#include <utility> // for move

#include "fmt/format.h"    // for format
#include "tl/expected.hpp" // for expected

tt::net::Socket::Socket(nng::socket &&socket, std::filesystem::path address)
    : socket_(std::move(socket)), socket_path_(std::move(address)) {}

auto tt::net::Socket::GetNetAddress() const -> std::string {
    return fmt::format("ipc://{}", socket_path_.string());
}
auto tt::net::Socket::socket() const -> nng::socket_view { return socket_; }

auto tt::net::Socket::ReceiveMessage()
    -> tl::expected<std::string, std::string> {
    try {
        auto buffer = socket_.recv();
        return std::string{static_cast<const char *>(buffer.data()),
                           buffer.size()};
    } catch (nng::exception &e) {
        return tl::make_unexpected(e.what());
    }
}

auto tt::net::Socket::SendMessage(const std::string &message)
    -> tl::expected<void, std::string> {
    try {
        nng::buffer msg = nng::make_buffer(message.size());
        memcpy(msg.data(), message.data(), message.size());
        socket_.send(msg);
        // Success
        return {};
    } catch (nng::exception &e) {
        return tl::make_unexpected(e.what());
    }
}
