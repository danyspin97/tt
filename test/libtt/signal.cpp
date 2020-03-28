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

#include "catch2/catch.hpp"
#include "libtt/signal.hpp"

using std::string;
using tt::Signal;
using tt::parseSignalFromString;

TEST_CASE("parseSignalFromString"){
    CHECK(parseSignalFromString("SIGHUP") == Signal::kSigHup);
    CHECK(parseSignalFromString("SIGINT") == Signal::kSigInt);
    CHECK(parseSignalFromString("SIGQUIT") == Signal::kSigQuit);
    CHECK(parseSignalFromString("SIGKILL") == Signal::kSigKill);
    CHECK(parseSignalFromString("SIGTERM") == Signal::kSigTerm);
}

TEST_CASE("parseSignalFromString with exception"){
    REQUIRE_THROWS(parseSignalFromString(""));
}

