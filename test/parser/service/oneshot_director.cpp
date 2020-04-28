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

#include "tt/parser/service/oneshot_director.hpp"

#include "catch2/catch.hpp"

#include "tt/data/oneshot.hpp"

using std::getline;
using std::ifstream;
using std::string;
using std::vector;

using tt::Oneshot;
using tt::OneshotDirector;
using tt::Service;

TEST_CASE("OneshotDirector") {
    auto director = OneshotDirector();

    ifstream service_file("../test/data/init-fsck.system");

    vector<string> lines;
    string line;
    while (getline(service_file, line)) {
        lines.push_back(line);
    }
    auto service = director.ParseAndGetService(lines, "/tmp/init-fsck");
    Oneshot *oneshot = static_cast<Oneshot *>(service.get());

    CHECK(oneshot->name() == "init-fsck");
    CHECK(oneshot->start().environment().Get("CMDARGS") == "-d");
}
