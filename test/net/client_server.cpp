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

#include <cstdint> // for uint16_t
#include <string>  // for basic_string, allocator, operator==

#include "catch2/catch.hpp" // for SourceLineInfo, AssertionHandler, Section

#include "tl/expected.hpp" // for expected

#include "tt/net/client.hpp" // for Client
#include "tt/net/server.hpp" // for Server
#include "tt/net/socket.hpp" // for Socket, Socket::Protocol, Socket::Proto...

TEST_CASE("Client/Server", "[!mayfail]") {
    SECTION("Send and receive text message") {
        constexpr const char *socket_path = "ipc-test.socket";
        tt::net::Server server(socket_path);
        tt::net::Client client(socket_path);
        REQUIRE_FALSE(server.IsListening());
        REQUIRE_NOTHROW(server.Listen());
        REQUIRE(server.IsListening());
        REQUIRE_FALSE(client.IsConnected());
        REQUIRE_NOTHROW(client.Connect());
        REQUIRE(client.IsConnected());

        std::string sent_message{"This is a simple message"};
        REQUIRE_NOTHROW(client.SendMessage(sent_message));

        auto received_message = server.ReceiveMessage();
        REQUIRE(received_message);
        CHECK(sent_message == received_message);
    }
}
