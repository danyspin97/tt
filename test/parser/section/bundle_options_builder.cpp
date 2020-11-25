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

#include "tt/parser/section/bundle_options_builder.hpp"

#include <initializer_list> // for initializer_list
#include <vector>           // for vector, operator==

#include "catch2/catch.hpp" // for SourceLineInfo, Section, oper...

#include "tt/data/bundle_options.hpp"  // for BundleOptions
#include "tt/parser/parser_error.hpp"  // for ParserError
#include "tt/parser/section/utils.hpp" // for TestBuilderWithFile

TEST_CASE("BundleOptionsBuilder") {
    tt::BundleOptionsBuilder builder;

    SECTION("Parse valid section") {
        auto ret =
            TestBuilderWithFile(builder, "../test/data/bundle_options_section");
        REQUIRE(ret.has_value());

        auto expected_contents = std::vector<std::string>{"foo", "bar"};
        auto expected_deps = std::vector<std::string>{"foobar"};
        REQUIRE(builder.options().contents() == expected_contents);
        REQUIRE(builder.options().dependencies() == expected_deps);
    }

    SECTION("Parse invalid section") {
        const std::vector<std::string> testFiles = {
            "empty_multiline_value", "invalid", "invalid_multiline_value",
            "unknown_multiline_value", "empty-file"};
        for (const auto &test : testFiles) {
            CHECK_FALSE(TestBuilderWithFile(builder,
                                            std::string{"../test/data/"} + test)
                            .has_value());
        }
    }
}
