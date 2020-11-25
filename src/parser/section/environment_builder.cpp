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

#include "tt/parser/section/environment_builder.hpp"

#include <cctype> // for isalnum, isdigit

#include "fmt/format.h" // for format

#include "tl/expected.hpp" // for expected

#include "tt/data/environment.hpp"             // for Environment
#include "tt/parser/line/exception.hpp"        // for KeyValueParserLineInv...
#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/parser_error.hpp"          // for ParserError
#include "tt/parser/section/exception.hpp"     // for EnvironmentKeyNotVali...
#include "tt/parser/section/utils.hpp"         // for IsEmptyLine
tt::EnvironmentBuilder::EnvironmentBuilder() noexcept
    : SectionBuilder("config") {}

auto tt::EnvironmentBuilder::ParseLine(const std::string &line) noexcept
    -> tl::expected<void, ParserError> {
    auto ret = TryParseLine(line);
    if (!ret) {
        return chain_parser_error<void>(ret.error(), " in [config] section");
    }
    return {};
}

auto tt::EnvironmentBuilder::TryParseLine(const std::string &line) noexcept
    -> tl::expected<void, ParserError> {
    if (IsEmptyLine(line)) {
        return {};
    }

    auto key_value_pair = KeyValueParser::ParseLine(line);
    if (!key_value_pair.has_value()) {
        return tl::unexpected(key_value_pair.error());
    }
    auto is_key_valid = CheckKeyIsValid(key_value_pair.value().first);
    if (!is_key_valid.has_value()) {
        return tl::unexpected(is_key_valid.error());
    }
    auto ret = environment_.SetUnique(key_value_pair.value().first,
                                      key_value_pair.value().second);
    if (!ret.has_value()) {
        return make_parser_error<void>(
            ParserError::Type::EnvironmentKeyAlreadySet, ret.error());
    }
    return {};
}

auto tt::EnvironmentBuilder::CheckKeyIsValid(const std::string &key) noexcept
    -> tl::expected<void, ParserError> {
    if (isdigit(key[0]) != 0) {
        return make_parser_error<void>(
            ParserError::Type::EnvironmentKeyNotValid,
            fmt::format("Key '{}' cannot start with a digit", key));
    }

    for (char c : key) {
        if ((isalnum(c) == 0) && c != '_') {
            return make_parser_error<void>(
                ParserError::Type::EnvironmentKeyNotValid,
                fmt::format("Character '{}' not valid in key '{}'",
                            std::string{c}, key));
        }
    }

    return {};
}

auto tt::EnvironmentBuilder::environment() const noexcept -> Environment {
    return environment_;
}

auto tt::EnvironmentBuilder::environment() noexcept -> Environment && {
    return std::move(environment_);
}

auto tt::EnvironmentBuilder::EndParsing() noexcept
    -> tl::expected<void, tt::ParserError> {
    return {};
}
