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

#include "tt/parser/service/service_director.hpp"

#include "tt/exception.hpp"
#include "tt/parser/line/section_line_parser.hpp"
#include "tt/parser/section/dummy_builder.hpp"
#include "tt/parser/section/section_builder.hpp"

using std::string;
using std::vector;

using tt::Exception;
using tt::SectionBuilder;
using tt::SectionLineParser;
using tt::ServiceDirector;

auto ServiceDirector::ParseAndGetService(vector<string> &service_lines,
                                         string &&path) -> tt::Service {
    service_lines_ = service_lines;
    ParseSections();
    return InstanceService(std::move(path));
}

void ServiceDirector::ParseSections() {
    try {
        TryParseSections();
    } catch (DummyBuilderException &e) {
        throw Exception("No line is allowed before [main] section");
    }
}

void ServiceDirector::TryParseSections() {
    SectionBuilder *current_builder = &dummy_builder_;
    for (auto &line : service_lines_) {
        auto section_line_parser = SectionLineParser(line);
        if (section_line_parser.IsLineValid()) {
            current_builder->EndParsing();
            current_builder =
                GetBuilderForSection(section_line_parser.section());

            continue;
        }

        current_builder->ParseLine(line);
    }
    current_builder->EndParsing();
}
