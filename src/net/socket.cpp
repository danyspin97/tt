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

#include "fmt/format.h" // for format

tt::net::Socket::Socket(nng::socket &&socket,
                        std::filesystem::path socket_path_)
    : socket_(std::move(socket)), socket_path_(std::move(socket_path_)) {}

auto tt::net::Socket::GetNetAddress() const -> std::string {
    return fmt::format("ipc://{}", socket_path_.string());
}
auto tt::net::Socket::socket() const -> nng::socket_view { return socket_; }
