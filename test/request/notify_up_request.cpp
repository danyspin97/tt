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

#include "tt/request/notify_up_request.hpp"

#include "catch2/catch.hpp" // for AssertionHandler, ope...

#include "tt/request/pack_request.hpp"   // for PackRequest
#include "tt/request/unpack_request.hpp" // for UnpackRequest

TEST_CASE("NotifyUpRequest") {
    SECTION("Convert to msgpack and back") {
        tt::NotifyUpRequest action("dummy", true);
        auto buffer = tt::PackRequest(action);

        auto deserialized_action = tt::UnpackRequest<tt::NotifyUpRequest>(buffer);
        CHECK(deserialized_action.name() == "notify_up");
        CHECK(deserialized_action.service() == action.service());
        CHECK(deserialized_action.succeded());
    }
}
