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

#include "tt/parser/service/bundle_director.hpp"

#include <variant> // for get

#include "catch2/catch.hpp" // for operator""_catch_sr, SourceLi...

#include "tt/data/bundle.hpp"          // for Oneshot
#include "tt/parser/parser_error.hpp"  // for ParserError
#include "tt/parser/service/utils.hpp" // for GetLinesFromFile

TEST_CASE("BundleDirector") {
    auto director = tt::BundleDirector();

    SECTION("Test init-fsck.system service") {
        auto lines = tt::GetLinesFromFile("../test/data/mount-all.system");
        std::vector<std::string> expected_contents{"mount-cgroup",
                                                   "mount-fstab"};
        std::vector<std::string> expected_deps{"init-rwfs"};
        auto service =
            director.ParseAndGetService(lines, "/tmp/mount-all.system");
        REQUIRE(service.has_value());
        auto bundle = std::get<tt::Bundle>(service.value());

        CHECK(bundle.name() == "mount-all");

        tt::BundleOptions options = bundle.options();
        CHECK(options.contents() == expected_contents);
        CHECK(options.dependencies() == expected_deps);
    }
}
