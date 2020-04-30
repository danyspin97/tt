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

#include "tt/parser/service/service_parser.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "tt/exception.hpp"
#include "tt/parser/line/key_value_parser.hpp"
#include "tt/parser/line/section_line_parser.hpp"
#include "tt/parser/service/parser_factory.hpp"
#include "tt/parser/service/service_director.hpp"

using std::ifstream;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

using tt::Exception;
using tt::KeyValueParser;
using tt::ParserFactory;
using tt::SectionLineParser;
using tt::Service;
using tt::ServiceParser;

vector<string> ServiceParser::GenerateListFrom(const string &path) {
    vector<string> lines;
    ifstream file{path};
    const auto size = std::filesystem::file_size(path);
    string section(size, ' ');
    file.read(section.data(), size);
    stringstream section_stream(section);
    string line;
    while (getline(section_stream, line, '\n')) {
        lines.push_back(line);
    }

    return lines;
}

ServiceParser::ServiceParser(const string &path) {
    path_ = path;
    auto service_lines = GenerateListFrom(path);
    const auto type = GetTypeFromService(service_lines);
    auto director = parser_factory_.GetDirectorPerType(type);
    service_ = director.get()->ParseAndGetService(service_lines, path);
}

const string
ServiceParser::GetTypeFromService(const vector<string> &service_lines) {
    auto main_section_index = GetMainSectionIndex(service_lines);
    for (auto i = main_section_index; i < service_lines.size(); i++) {
        auto key_value_parser = KeyValueParser(service_lines.at(i));
        if (key_value_parser.IsLineValid() &&
            key_value_parser.key() == "type") {
            return key_value_parser.value();
        }
    }
    auto msg = "No type data could be found for service " + path_;
    throw Exception(msg);
}

size_t
ServiceParser::GetMainSectionIndex(const vector<string> &service_lines) const {
    size_t index = 0;
    for (auto &line : service_lines) {
        auto section_line_parser = SectionLineParser(line);
        auto const current_section =
            section_line_parser.GetSectionOrDefault("");
        if (current_section == "main") {
            return index;
        }
        index++;
    }

    auto msg = "No main section could be found for service " + path_;
    throw Exception(msg);
}

shared_ptr<Service> &ServiceParser::service() { return service_; }
