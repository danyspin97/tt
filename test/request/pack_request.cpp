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

#include "tt/request/pack_request.hpp"

#include "catch2/catch.hpp" // for AssertionHandler, ope...

#include "tt/request/notify_service_status.hpp" // for NotifyServiceStatus

TEST_CASE("PackAction") {
    SECTION("Convert to json") {
        tt::request::NotifyServiceStatus request("dummy",
                                                 tt::ServiceStatus::Up);
        auto buffer = tt::request::PackRequest(request);

        nlohmann::json j = nlohmann::json::parse(buffer);

        CHECK(j.at("request_name") == tt::request::NotifyServiceStatus::name);
        CHECK(j.at("request").at("service") == "dummy");
        CHECK(j.at("request").at("status") ==
              tt::GetServiceStatusName(tt::ServiceStatus::Up));
    }
}
