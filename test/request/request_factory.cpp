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

#include "tt/request/request_factory.hpp"

#include "catch2/catch.hpp" // for operator""_catch_sr

#include "tt/request/notify_up_request.hpp" // for NotifyUpRequest
#include "tt/request/pack_request.hpp"      // for PackRequest
#include "tt/request/request.hpp"           // for Request

TEST_CASE("RequestFactory") {
    SECTION("NotifyUpRequest") {
        tt::NotifyUpRequest action("dummy", true);
        auto buffer = tt::PackRequest(action);

        auto deserialized_action =
            tt::RequestFactory::GetRequestFromBuffer(buffer);

        REQUIRE(deserialized_action);
        CHECK(deserialized_action->name() == "notify_up");
        CHECK_NOTHROW(
            dynamic_cast<tt::NotifyUpRequest *>(deserialized_action.get()));
    }
}
