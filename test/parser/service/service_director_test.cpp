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

#include "tt/test/parser/service/service_director_test.hpp"

#include "catch2/catch.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

using tt::ServiceDirectorTest;

TEST_CASE("ServiceDirector") {
    auto director = ServiceDirectorTest();

    SECTION("Check number of parsed lines") {
        vector<string> lines = {"", "[foo]"};
        const int linesNumber = 10;
        for (int i = 0; i < linesNumber; i++) {
            lines.emplace_back("");
        }

        auto ret = director.ParseAndGetService(lines, "");
        REQUIRE(ret.has_value());
        CHECK(director.builder_test_.time_parsed_ == linesNumber);
        CHECK(director.builder_test_.SectionParsed());
    }

    SECTION("Parse invalid file") {
        vector<string> lines = {"foo", ""};
        CHECK_FALSE(director.ParseAndGetService(lines, "").has_value());
    }
}
