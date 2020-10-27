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
#include "tt/net/server.hpp"

#include "catch2/catch.hpp"

TEST_CASE("Client/Server") {
    SECTION("Send and receive text message") {
        constexpr const char *address = "localhost";
        constexpr const uint16_t port = 8999;
        constexpr tt::net::Socket::Protocol protocol =
            tt::net::Socket::Protocol::TCP;
        tt::net::Server server(protocol, address, port);
        tt::net::Client client(protocol, address, port);

        server.Listen();
        client.Connect();

        std::string sent_message{"This is a simple message"};
        client.SendMessage(sent_message);

        auto received_message = server.ReceiveMessage();
        CHECK(sent_message == received_message);
    }
}
