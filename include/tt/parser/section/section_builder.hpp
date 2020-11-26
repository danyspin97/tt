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

#ifndef TT_SECTION_BUILDER_HPP_
#define TT_SECTION_BUILDER_HPP_

#include <string>        // for string
#include <unordered_map> // for unordered_map
#include <vector>        // for vector

#include "tt/parser/line/array_parser.hpp" // for ArrayParser

namespace tl {
template <typename T, typename Z> class expected;
}

namespace tt {

class ParserError;

class SectionBuilder {
public:
    explicit SectionBuilder(std::string section);
    SectionBuilder() = delete;
    virtual ~SectionBuilder() = default;

    [[nodiscard]] virtual auto ParseLine(const std::string &line) noexcept
        -> tl::expected<void, ParserError>;
    [[nodiscard]] virtual auto EndParsing() noexcept
        -> tl::expected<void, ParserError>;

    [[nodiscard]] auto SectionParsed() const -> bool;

protected:
    [[nodiscard]] auto ParseMultilineValue(const std::string &line) noexcept
        -> tl::expected<bool, ParserError>;

    [[nodiscard]] auto GetAttribute(const std::string &key) const noexcept
        -> std::optional<std::string>;
    [[nodiscard]] auto GetAttribute(const std::string &key) noexcept
        -> std::optional<std::string>;
    [[nodiscard]] auto SetAttribute(std::string key, std::string value) noexcept
        -> tl::expected<void, ParserError>;
    [[nodiscard]] auto GetArrayAttribute(const std::string &key) const noexcept
        -> std::optional<std::vector<std::string>>;
    [[nodiscard]] auto GetArrayAttribute(const std::string &key) noexcept
        -> std::optional<std::vector<std::string>>;
    [[nodiscard]] auto
    SetArrayAttribute(std::string key, std::vector<std::string> value) noexcept
        -> tl::expected<void, ParserError>;

    [[nodiscard]] virtual auto GetValidAttributes() const noexcept
        -> std::vector<std::string> = 0;
    [[nodiscard]] virtual auto GetValidArrayAttributes() const noexcept
        -> std::vector<std::string> = 0;

    [[nodiscard]] auto section() const noexcept -> std::string {
        return section_;
    }

private:
    bool finished_ = false;
    std::string section_;
    std::unordered_map<std::string, std::string> values_;
    std::unordered_map<std::string, std::vector<std::string>> array_values_;
    ArrayParser array_parser_;
};

} // namespace tt

#endif // TT_SECTION_BUILDER_HPP_
