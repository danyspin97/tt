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

#include "tt/parser/section/logger_script_builder.hpp"

#include "catch2/catch.hpp"

#include "tt/define.h"
#include "tt/parser/section/utils.hpp"

using tt::kDefaultLogGroup;
using tt::kDefaultLogUser;
using tt::LoggerScript;
using tt::LoggerScriptBuilder;

TEST_CASE("LoggerScriptBuilder") {
    auto builder = LoggerScriptBuilder("foo");

    SECTION("Parse valid section") {
        TestBuilderWithFile(builder, "../test/data/logger_script");
        LoggerScript s = builder.logger_script();

        CHECK(s.service_to_log() == "foo");

        const auto *const expected_execute = "s6-log /var/log/tt/foo";

        CHECK(s.execute() == expected_execute);
        CHECK(s.user() == "tt_log");
        CHECK(s.group() == "tt_log");
    }

    SECTION("Paarse section without execute script") {
        TestBuilderWithFile(builder, "../test/data/logger_script_no_execute");
        LoggerScript s = builder.logger_script();

        CHECK(s.service_to_log() == "foo");
        CHECK(s.user() == kDefaultLogUser);
        CHECK(s.group() == kDefaultLogGroup);
    }
}
