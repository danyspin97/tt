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

#include "tt/parser/section/script_builder.hpp"

#include <optional> // for optional

#include "catch2/catch.hpp" // for operator""_catch_sr, SourceLi...

#include "tt/parser/parser_error.hpp"  // for ParserError
#include "tt/parser/section/utils.hpp" // for TestBuilderWithFile
#include "tt/signal.hpp"               // for Signal, Signal::kSigKill

TEST_CASE("ScriptBuilder") {
    auto builder = tt::ScriptBuilder("test");
    SECTION("Parse valid script section") {
        auto ret = TestBuilderWithFile(builder, "../test/data/script_section");
        REQUIRE(ret.has_value());
        tt::Script s = builder.script();

        REQUIRE(s.user().has_value());
        CHECK(s.user().value() == "dbus");
        REQUIRE(s.group().has_value());
        CHECK(s.group().value() == "dbus");
        CHECK(s.timeout() == 5000);
        CHECK(s.timeout_kill() == 10);
        CHECK(s.down_signal() == tt::Signal::kSigKill);
        CHECK(s.max_death() == 250);
    }

    SECTION("Parse invalid line") {
        CHECK_FALSE(builder.ParseLine("invalid").has_value());
    }
}
