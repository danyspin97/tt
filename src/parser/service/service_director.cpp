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

#include "fmt/format.h" // for format

#include "tl/expected.hpp" // for expected

#include "tt/exception.hpp"                       // for Exception
#include "tt/parser/line/section_line_parser.hpp" // for SectionLineParser
#include "tt/parser/parser_error.hpp"             // for ParserError
#include "tt/parser/section/dummy_builder.hpp"    // for DummyBuilder
#include "tt/parser/section/exception.hpp"        // for DummyBuilderException
#include "tt/parser/section/section_builder.hpp"  // for SectionBuilder

auto tt::ServiceDirector::ParseAndGetService(
    std::vector<std::string> service_lines, std::string path)
    -> tl::expected<Service, ParserError> {
    service_lines_ = std::move(service_lines);
    if (auto ret = ParseSections(); !ret.has_value()) {
        return tl::make_unexpected(ret.error());
    }
    return InstanceService(std::move(path));
}

auto tt::ServiceDirector::ParseSections() -> tl::expected<void, ParserError> {
    SectionBuilder *current_builder = &dummy_builder_;
    auto line = service_lines_.begin();
    while (line != service_lines_.end()) {
        auto section_line_parser = SectionLineParser(*line);
        if (section_line_parser.IsLineValid()) {
            if (auto ret = current_builder->EndParsing(); !ret.has_value()) {
                return ret;
            }
            auto ret = GetBuilderForSection(section_line_parser.section());
            if (!ret.has_value()) {
                return chain_parser_error<void>(
                    std::move(ret.error()),
                    fmt::format(" in line {}",
                                std::distance(service_lines_.begin(), line)));
            }

            current_builder = ret.value();
            line++;
            continue;
        }

        if (auto ret = current_builder->ParseLine(*line); !ret.has_value()) {
            return chain_parser_error<void>(
                std::move(ret.error()),
                fmt::format(" in line {}",
                            std::distance(service_lines_.begin(), line)));
        }

        line++;
    }
    return current_builder->EndParsing();
}
