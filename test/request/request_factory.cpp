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

#include "tt/request/notify_service_status.hpp" // for NotifyServiceStatus
#include "tt/request/pack_request.hpp"          // for PackAction
#include "tt/request/request.hpp"               // for Action

TEST_CASE("ActionFactory") {
    SECTION("NotifyServiceStatus") {
        tt::request::NotifyServiceStatus request("dummy",
                                                 tt::ServiceStatus::Up);
        auto buffer = tt::request::PackRequest(request);

        auto deserialized_request =
            tt::request::RequestFactory::GetRequestFromBuffer(buffer);

        CHECK_NOTHROW(std::is_same_v<
                      std::decay_t<decltype(deserialized_request.second.get())>,
                      tt::request::NotifyServiceStatus>);
        CHECK(deserialized_request.first ==
              tt::request::NotifyServiceStatus::request_name);
    }
}
