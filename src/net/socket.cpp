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

#include <cassert> // for assert
#include <utility> // for move

#include "fmt/format.h"          // for format
#include "nngpp/protocol/rep0.h" // for req0::open
#include "nngpp/protocol/req0.h" // for rep0::open
#include "tl/expected.hpp"       // for expected

tt::net::Socket::Socket(Mode mode, std::filesystem::path address)
    : mode_(mode), socket_path_(std::move(address)) {}

auto tt::net::Socket::GetNetAddress() const -> std::string {
    return fmt::format("ipc://{}", socket_path_.string());
}

auto tt::net::Socket::Connect() -> tl::expected<void, std::string> {
    if (is_connected_) {
        return {};
    }

    try {
        switch (mode_) {
        case Mode::Client:
            socket_ = nng::req::open();
            socket_.dial(GetNetAddress().c_str());
            break;
        case Mode::Server:
            socket_ = nng::rep::open();
            socket_.listen(GetNetAddress().c_str());
            break;
        default:
            assert(false);
        }
        is_connected_ = true;
    } catch (nng::exception &e) {
        return tl::make_unexpected(e.what());
    }
    return {};
}

auto tt::net::Socket::ReceiveMessage()
    -> tl::expected<std::string, std::string> {
    if (!is_connected_) {
        return tl::make_unexpected("Socket is not connected");
    }

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
    if (!is_connected_) {
        return tl::make_unexpected("Socket is not connected");
    }

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

auto tt::net::Socket::IsConnected() const noexcept -> bool {
    return is_connected_;
}
