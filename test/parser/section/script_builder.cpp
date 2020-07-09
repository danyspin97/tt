/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include "catch2/catch.hpp"

#include "tt/parser/section/utils.hpp"

TEST_CASE("ScriptBuilder") {
    auto builder = tt::ScriptBuilder("test");
    SECTION("Parse valid script section") {
        TestBuilderWithFile(builder, "../test/data/script_section");
        tt::Script s = builder.script();

        CHECK(s.user().value() == "dbus");
        CHECK(s.group().value() == "dbus");
        CHECK(s.timeout() == 5000);
        CHECK(s.timeout_kill() == 10);
        CHECK(s.down_signal() == tt::Signal::kSigKill);
        CHECK(s.max_death() == 250);
    }

    SECTION("Parse invalid line") {
        REQUIRE_THROWS(builder.ParseLine("invalid"));
    }
}
