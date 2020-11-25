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

#include "tt/parser/service/oneshot_director.hpp"

#include <variant> // for get

#include "catch2/catch.hpp" // for operator""_catch_sr, SourceLi...

#include "tt/data/oneshot.hpp"         // for Oneshot
#include "tt/parser/parser_error.hpp"  // for ParserError
#include "tt/parser/service/utils.hpp" // for GetLinesFromFile

TEST_CASE("OneshotDirector") {
    auto director = tt::OneshotDirector();

    SECTION("Test init-fsck.system service") {
        auto lines = tt::GetLinesFromFile("../test/data/init-fsck.system");
        auto service = director.ParseAndGetService(lines, "/tmp/init-fsck");
        REQUIRE(service.has_value());
        auto oneshot = std::get<tt::Oneshot>(service.value());

        CHECK(oneshot.name() == "init-fsck");
        CHECK(oneshot.environment().Get("CMDARGS") == "-d");
    }

    SECTION("Test service without [start] section") {
        auto lines = tt::GetLinesFromFile("../test/data/nostart");
        CHECK_FALSE(
            director.ParseAndGetService(lines, "/tmp/nostart").has_value());
    }

    SECTION("Test service with unknown section") {
        auto lines = tt::GetLinesFromFile("../test/data/unknown_section");
        CHECK_FALSE(
            director.ParseAndGetService(lines, "/tmp/nostart").has_value());
    }
}
