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

#include "tt/parser/section/longrun_options_builder.hpp"

#include <initializer_list> // for initializer_list
#include <vector>           // for vector, operator==

#include "catch2/catch.hpp" // for SourceLineInfo, operator""_ca...

#include "tt/data/longrun_options.hpp" // for LongrunOptions
#include "tt/parser/section/utils.hpp" // for TestBuilderWithFile

namespace tt {
class SectionBuilderException;
} // namespace tt

using std::string;
using std::vector;

using tt::LongrunOptions;
using tt::LongrunOptionsBuilder;
using tt::SectionBuilderException;
using tt::TestBuilderWithFile;

TEST_CASE("LongrunOptionsBuilder") {
    auto options = LongrunOptions();
    auto builder = LongrunOptionsBuilder(options);

    SECTION("Parse valid section") {
        TestBuilderWithFile(builder, "../test/data/longrun_options_section");

        auto expected_deps = vector<string>{"foo", "bar"};

        CHECK(options.dependencies() == expected_deps);
        CHECK(options.optional() == true);
    }

    SECTION("Parse invalid sections") {
        const auto testFiles = {"empty_multiline_value",
                                "invalid",
                                "invalid_boolean",
                                "invalid_int_value",
                                "invalid_multiline_value",
                                "invalid_quotes",
                                "unclosed_quotes",
                                "unclosed_multiline_value",
                                "unknown_key",
                                "unknown_multiline_value"};
        for (const auto &test : testFiles) {
            CHECK_THROWS_AS(
                TestBuilderWithFile(builder, string{"../test/data/"} + test),
                SectionBuilderException);
        }
    }
}
