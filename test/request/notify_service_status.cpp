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

#include "tt/request/notify_service_status.hpp"

#include "catch2/catch.hpp" // for AssertionHandler, ope...

#include <nlohmann/json.hpp> // for json

#include "tt/request/adapter/notify_service_statup.hpp" // IWYU pragma: keep

TEST_CASE("NotifyUpRequest") {
    SECTION("Convert to json and back") {
        tt::request::NotifyServiceStatus request("dummy",
                                                 tt::ServiceStatus::Up);
        nlohmann::json j = request;

        std::unique_ptr<tt::request::NotifyServiceStatus> deserialized_request;
        REQUIRE_NOTHROW(deserialized_request =
                            std::make_unique<tt::request::NotifyServiceStatus>(
                                j.get<tt::request::NotifyServiceStatus>()));
        CHECK(deserialized_request->service() == request.service());
        CHECK(deserialized_request->status() == tt::ServiceStatus::Up);
    }
}
