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

#include "libtt/parser/section/utils.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "libtt/parser/section/exception.hpp"
#include "libtt/parser/section/section_builder.hpp"

using std::string;

void tt::TestBuilderWithFile(tt::SectionBuilder &builder, const string path) {
    // https://stackoverflow.com/a/40903508
    std::ifstream f{path};
    const auto size = std::filesystem::file_size(path);
    string section(size, ' ');
    f.read(section.data(), size);
    std::stringstream section_stream(section);
    string line;
    while (std::getline(section_stream, line, '\n')) {
        builder.ParseLine(line);
    }
    builder.EndParsing();
}

void tt::AttributeNotFound(const string attribute, const string section) {
    const auto error_message = "Camp named" + attribute +
                               " is not allowed in section [" + section + "]";
    throw tt::AttributeNotFoundException(error_message);
}
