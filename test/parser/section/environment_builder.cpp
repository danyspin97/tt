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

#include "tt/parser/section/environment_builder.hpp"

#include <map>     // for allocator, map, map<>::mapped...
#include <utility> // for pair

#include "catch2/catch.hpp" // for SourceLineInfo, operator""_ca...

#include "tt/data/environment.hpp"     // for Environment
#include "tt/parser/section/utils.hpp" // for TestBuilderWithFile

#include "tt/parser/parser_error.hpp" // for ParserError

TEST_CASE("EnvironmentBuilder") {
    tt::EnvironmentBuilder builder;

    SECTION("Parse valid section") {
        auto ret =
            TestBuilderWithFile(builder, "../test/data/environment_section");
        REQUIRE(ret.has_value());
        std::map<std::string, std::string> expected;
        expected["CMDARGS"] = "-d --nofork";
        expected["LOGLEVEL"] = "0";
        for (auto &pair : expected) {
            CHECK(builder.environment().Get(pair.first) == pair.second);
        }
    }

    SECTION("Parse invalid section") {
        CHECK_FALSE(
            TestBuilderWithFile(builder, "../test/data/invalid").has_value());
        CHECK_FALSE(
            TestBuilderWithFile(builder, "../test/data/invalid_environment")
                .has_value());
    }
}
