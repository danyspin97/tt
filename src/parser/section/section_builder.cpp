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

#include "tt/parser/section/section_builder.hpp"

#include "fmt/format.h" // for format

#include "tt/parser/line/key_value_parser.hpp" // for KeyValueParser
#include "tt/parser/parser_error.hpp"          // for ParserError
#include "tt/parser/section/utils.hpp"         // for IsEmptyLine

tt::SectionBuilder::SectionBuilder(std::string section)
    : section_(std::move(section)) {}

auto tt::SectionBuilder::ParseLine(const std::string &line) noexcept
    -> tl::expected<void, ParserError> {
    const std::string section_err_msg =
        fmt::format(" in [{}] section", section_);
    if (IsEmptyLine(line)) {
        return {};
    }

    auto ret = ParseMultilineValue(line);
    // The multiline SetArrayAttributeis invalid/has errored
    if (!ret.has_value()) {
        return chain_parser_error<void>(ret.error(), section_err_msg);
    }

    // We have found a multiline value and we have also parsed it
    if (ret.value()) {
        return {};
    }

    auto key_value_pair = KeyValueParser::ParseLine(line);
    if (key_value_pair.has_value()) {
        if (auto ret = SetAttribute(std::move(key_value_pair.value().first),
                                    std::move(key_value_pair.value().second));
            !ret.has_value()) {
            return chain_parser_error<void>(ret.error(), section_err_msg);
        }
        return {};
    }

    return tl::unexpected(key_value_pair.error());
}

auto tt::SectionBuilder::EndParsing() noexcept
    -> tl::expected<void, ParserError> {
    if (array_parser_.IsParsing()) {
        return make_parser_error<void>(
            ParserError::Type::ArrayValueNotClosed,
            fmt::format("'{}' has not been closed in [{}] section",
                        array_parser_.key(), section_));
    }

    finished_ = true;
    return {};
}

auto tt::SectionBuilder::ParseMultilineValue(const std::string &line) noexcept
    -> tl::expected<bool, ParserError> {
    if (array_parser_.IsParsing()) {
        if (auto ret = array_parser_.ParseLine(line); !ret.has_value()) {
            return tl::unexpected(ret.error());
        }

        if (!array_parser_.IsParsing()) {
            if (auto ret = SetArrayAttribute(std::move(array_parser_.key()),
                                             std::move(array_parser_.values()));
                !ret.has_value()) {
                return tl::unexpected(ret.error());
            }
            array_parser_.Reset();
        }
        return true;
    }

    auto ret = array_parser_.StartParsing(line);
    if (!ret.has_value()) {
        return tl::unexpected(ret.error());
    }
    if (ret.value()) {
        if (!array_parser_.IsParsing()) {
            if (auto ret = SetArrayAttribute(std::move(array_parser_.key()),
                                             std::move(array_parser_.values()));
                !ret.has_value()) {
                return tl::unexpected(ret.error());
            }
            array_parser_.Reset();
        }
        return true;
    }

    return false;
}

auto tt::SectionBuilder::SetAttribute(std::string key,
                                      std::string value) noexcept
    -> tl::expected<void, ParserError> {
    auto valid_keys = GetValidAttributes();
    if (std::find(valid_keys.begin(), valid_keys.end(), key) ==
        valid_keys.end()) {
        return make_parser_error<void>(
            ParserError::Type::InvalidKeyInSection,
            fmt::format("'{}' is not a valid key", key));
    }

    if (values_.find(key) != values_.end()) {
        return make_parser_error<void>(
            ParserError::Type::SectionKeyAlreadySet,
            fmt::format("'{}' has already been set", key));
    }

    values_.emplace(std::move(key), std::move(value));
    return {};
}

auto tt::SectionBuilder::GetAttribute(const std::string &key) const noexcept
    -> std::optional<std::string> {
    if (auto value = values_.find(key); value != values_.end()) {
        return value->second;
    }

    return {};
}

auto tt::SectionBuilder::GetAttribute(const std::string &key) noexcept
    -> std::optional<std::string> {
    if (auto value = values_.find(key); value != values_.end()) {
        return std::move(value->second);
    }

    return {};
}

auto tt::SectionBuilder::SetArrayAttribute(
    std::string key, std::vector<std::string> value) noexcept
    -> tl::expected<void, ParserError> {
    auto valid_keys = GetValidArrayAttributes();
    if (std::find(valid_keys.begin(), valid_keys.end(), key) ==
        valid_keys.end()) {
        return make_parser_error<void>(
            ParserError::Type::InvalidKeyInSection,
            fmt::format("'{}' is not a valid key", key));
    }

    if (array_values_.find(key) != array_values_.end()) {
        return make_parser_error<void>(
            ParserError::Type::SectionKeyAlreadySet,
            fmt::format("'{}' has already been set", key));
    }

    array_values_.emplace(std::move(key), std::move(value));
    return {};
}

auto tt::SectionBuilder::GetArrayAttribute(const std::string &key)
    const noexcept -> std::optional<std::vector<std::string>> {
    if (auto value = array_values_.find(key); value != array_values_.end()) {
        return value->second;
    }

    return {};
}

auto tt::SectionBuilder::GetArrayAttribute(const std::string &key) noexcept
    -> std::optional<std::vector<std::string>> {
    if (auto value = array_values_.find(key); value != array_values_.end()) {
        return std::move(value->second);
    }

    return {};
}

auto tt::SectionBuilder::SectionParsed() const -> bool { return finished_; }
