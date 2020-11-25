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

#include "fmt/format.h" // for format

#include "tt/parser/parser_error.hpp"            //for ParserError
#include "tt/parser/section/section_builder.hpp" // for SectionBuilder

auto tt::TestBuilderWithFile(SectionBuilder &builder, const std::string &path)
    -> tl::expected<void, ParserError> {
    // https://stackoverflow.com/a/40903508
    std::ifstream f{path};
    const auto size = std::filesystem::file_size(path);
    std::string section(size, ' ');
    f.read(section.data(), size);
    std::stringstream section_stream(section);
    std::string line;
    while (getline(section_stream, line, '\n')) {
        auto ret = builder.ParseLine(line);
        if (!ret.has_value()) {
            return chain_parser_error<void>(std::move(ret.error()), "");
        }
    }
    return builder.EndParsing();
}

auto tt::KeyNotFound(const std::string &key, const std::string &section)
    -> tl::expected<void, ParserError> {
    return make_parser_error<void>(
        ParserError::Type::InvalidKeyInSection,
        fmt::format("Key '{}' is not allowed in [{}] section", key, section));
}
