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

#pragma once

#include "tt/net/socket.hpp" // for Socket, Socket::Protocol

namespace tt::net {

class Server : public Socket {
public:
    explicit Server(std::filesystem::path socket_path);
    // Start listening on this socket
    // Return false on error, true otherwise
    auto Listen() -> bool;
    [[nodiscard]] auto IsListening() const noexcept -> bool;

    auto ReceiveMessage() -> std::string;
private:
    bool is_listening_ = false;
};

} // namespace tt::net
