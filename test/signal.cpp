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

#include "tt/signal.hpp"

#include "catch2/catch.hpp"

#include "tl/expected.hpp" // for expected

using std::string;
using tt::ParseSignalFromString;
using tt::Signal;

TEST_CASE("ParseSignalFromString") {
    CHECK(ParseSignalFromString("SIGHUP").value() == Signal::kSigHup);
    CHECK(ParseSignalFromString("SIGINT").value() == Signal::kSigInt);
    CHECK(ParseSignalFromString("SIGQUIT").value() == Signal::kSigQuit);
    CHECK(ParseSignalFromString("SIGKILL").value() == Signal::kSigKill);
    CHECK(ParseSignalFromString("SIGTERM").value() == Signal::kSigTerm);
}

TEST_CASE("GetSignalName") {
    CHECK("SIGHUP" == tt::GetSignalName(Signal::kSigHup));
    CHECK("SIGINT" == tt::GetSignalName(Signal::kSigInt));
    CHECK("SIGQUIT" == tt::GetSignalName(Signal::kSigQuit));
    CHECK("SIGKILL" == tt::GetSignalName(Signal::kSigKill));
    CHECK("SIGTERM" == tt::GetSignalName(Signal::kSigTerm));
}

TEST_CASE("ParseSignalFromString with exception") {
    REQUIRE_FALSE(ParseSignalFromString("").has_value());
}
