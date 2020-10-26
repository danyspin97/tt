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

#include "tt/parser/section/utils.hpp"

#include <filesystem> // for file_size
#include <fstream>    // for basic_istream, ifst...
#include <sstream>    // for stringstream
#include <string>     // for allocator, string

#include "tt/parser/section/exception.hpp"       // for AttributeNotFoundEx...
#include "tt/parser/section/section_builder.hpp" // for SectionBuilder

using std::getline;
using std::ifstream;
using std::string;
using std::stringstream;

using tt::AttributeNotFoundException;
using tt::SectionBuilder;

void tt::TestBuilderWithFile(SectionBuilder &builder, const string &path) {
    // https://stackoverflow.com/a/40903508
    ifstream f{path};
    const auto size = std::filesystem::file_size(path);
    string section(size, ' ');
    f.read(section.data(), size);
    stringstream section_stream(section);
    string line;
    while (getline(section_stream, line, '\n')) {
        builder.ParseLine(line);
    }
    builder.EndParsing();
}

void tt::AttributeNotFound(const string &attribute, const string &section) {
    const auto error_message =
        "Key '" + attribute + "' is not allowed in section [" + section + "]";
    throw AttributeNotFoundException(error_message);
}
