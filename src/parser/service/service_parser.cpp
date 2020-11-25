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

#include "tt/parser/service/service_parser.hpp"

#include <filesystem> // for file_size
#include <fstream>    // for basic_istream, ifs...
#include <memory>     // for unique_ptr
#include <sstream>    // for stringstream
#include <string>     // for string, allocator
#include <utility>    // for move

#include "tt/parser/line/key_value_parser.hpp"    // for KeyValueParser
#include "tt/parser/line/section_line_parser.hpp" // for SectionLineParser
#include "tt/parser/parser_error.hpp"             // for ParserError
#include "tt/parser/service/parser_factory.hpp"   // for ParserFactory
#include "tt/parser/service/service_director.hpp" // for ServiceDirector

auto tt::ServiceParser::GenerateListFrom(const std::string &path)
    -> std::vector<std::string> {
    std::vector<std::string> lines;
    std::ifstream file{path};
    const auto size = std::filesystem::file_size(path);
    std::string section(size, ' ');
    file.read(section.data(), size);
    std::stringstream section_stream(section);
    std::string line;
    while (getline(section_stream, line, '\n')) {
        lines.push_back(line);
    }

    return lines;
}

auto tt::ServiceParser::ParseService(std::string path)
    -> tl::expected<Service, ParserError> {
    auto service_lines = GenerateListFrom(path);
    auto type = GetTypeFromService(service_lines);
    if (!type.has_value()) {
        return tl::unexpected(type.error());
    }
    auto director = tt::ParserFactory::GetDirectorPerType(type.value());
    if (!director.has_value()) {
        return tl::unexpected(director.error());
    }
    return director.value()->ParseAndGetService(std::move(service_lines),
                                                std::move(path));
}

auto tt::ServiceParser::GetTypeFromService(
    const std::vector<std::string> &service_lines)
    -> tl::expected<std::string, ParserError> {
    auto main_section_index = GetMainSectionIndex(service_lines);
    if (!main_section_index.has_value()) {
        return tl::unexpected(main_section_index.error());
    }

    for (auto i = main_section_index.value(); i < service_lines.size(); i++) {
        auto key_value_pair = KeyValueParser::ParseLine(service_lines.at(i));
        if (key_value_pair.has_value() &&
            key_value_pair.value().first == "type") {
            return std::move(key_value_pair.value().second);
        }
    }

    return make_parser_error<std::string>(ParserError::Type::MissingServiceType,
                                          "Could not find service type");
}

auto tt::ServiceParser::GetMainSectionIndex(
    const std::vector<std::string> &service_lines)
    -> tl::expected<size_t, ParserError> {
    size_t index = 0;
    for (const auto &line : service_lines) {
        auto section_line_parser = SectionLineParser(line);
        auto const current_section =
            section_line_parser.GetSectionOrDefault("");
        if (current_section == "main") {
            return index;
        }
        index++;
    }

    return make_parser_error<size_t>(ParserError::Type::MissingMainSection,
                                     "No main section could be found");
}
