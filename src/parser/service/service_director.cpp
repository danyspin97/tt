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

#include "tt/parser/service/service_director.hpp"

#include <utility> // for move

#include "tt/exception.hpp"                       // for Exception
#include "tt/parser/line/section_line_parser.hpp" // for SectionLineParser
#include "tt/parser/section/dummy_builder.hpp"    // for DummyBuilder
#include "tt/parser/section/exception.hpp"        // for DummyBuilderException
#include "tt/parser/section/section_builder.hpp"  // for SectionBuilder

auto tt::ServiceDirector::ParseAndGetService(
    const std::vector<std::string> &service_lines, std::string &&path)
    -> tt::Service {
    service_lines_ = service_lines;
    ParseSections();
    return InstanceService(std::move(path));
}

void tt::ServiceDirector::ParseSections() {
    try {
        TryParseSections();
    } catch (const DummyBuilderException & /*e*/) {
        throw Exception("No line is allowed before [main] section");
    }
}

void tt::ServiceDirector::TryParseSections() {
    SectionBuilder *current_builder = &dummy_builder_;
    for (const auto &line : service_lines_) {
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
