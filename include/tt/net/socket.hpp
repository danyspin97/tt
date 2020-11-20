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

#include <filesystem> // for path

#include "nngpp/socket.h"      // for socket
#include "nngpp/socket_view.h" // for socket_view

namespace tl {
template <typename T, typename Z> class expected;
}

namespace tt::net {

class Socket {
public:
    enum class Mode {
        Client = 0,
        Server = 1,
    };

    Socket() = delete;
    ~Socket() = default;

protected:
    Socket(Mode, std::filesystem::path address);

public:
    auto SendMessage(const std::string &message)
        -> tl::expected<void, std::string>;
    auto ReceiveMessage() -> tl::expected<std::string, std::string>;

protected:
    auto Connect() -> tl::expected<void, std::string>;
    [[nodiscard]] auto IsConnected() const noexcept -> bool;

    [[nodiscard]] auto socket() -> nng::socket_view;

private:
    [[nodiscard]] auto GetNetAddress() const -> std::string;

    Mode mode_;
    bool is_connected_ = false;
    nng::socket socket_;
    std::filesystem::path socket_path_;
};

} // namespace tt::net
