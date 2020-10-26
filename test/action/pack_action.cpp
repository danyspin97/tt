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

#include "tt/action/pack_action.hpp"

#include "catch2/catch.hpp"

#include "tt/action/notify_up_action.hpp"
#include "tt/action/pack_action.hpp"

TEST_CASE("PackAction") {
    SECTION("Convert to msgpack") {
        tt::NotifyUpAction action("dummy", true);
        auto buffer = tt::PackAction(action);

        msgpack::object_handle result;
        msgpack::unpack(result, buffer.c_str(), buffer.size());
        msgpack::object obj(result.get());

        CHECK(obj.via.map.ptr[0].key.as<std::string>() == "Action");
        CHECK(obj.via.map.ptr[0].val.via.map.ptr[0].key.as<std::string>() ==
              "name_");
        CHECK(obj.via.map.ptr[0].val.via.map.ptr[0].val.as<std::string>() ==
              "notify_up");
        CHECK(obj.via.map.ptr[1].key.as<std::string>() == "service_");
        CHECK(obj.via.map.ptr[1].val.as<std::string>() == "dummy");
        CHECK(obj.via.map.ptr[2].key.as<std::string>() == "succeeded_");
        CHECK(obj.via.map.ptr[2].val.as<bool>() == true);
    }
}
