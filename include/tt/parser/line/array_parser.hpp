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

#ifndef TT_ARRAY_PARSER_HPP_
#define TT_ARRAY_PARSER_HPP_

#include <string>
#include <vector>

namespace tl {
template <typename T, typename Z> class expected;
}

namespace tt {

class ParserError;

class ArrayParser {
public:
    [[nodiscard]] auto IsParsing() const noexcept -> bool {
        return is_parsing_;
    }

    [[nodiscard]] auto key() const noexcept -> std::string { return key_; }
    [[nodiscard]] auto key() noexcept -> std::string { return std::move(key_); }

    [[nodiscard]] auto values() const noexcept -> std::vector<std::string> {
        return values_;
    }
    [[nodiscard]] auto values() noexcept -> std::vector<std::string> {
        return std::move(values_);
    }

    ArrayParser() = default;

    auto StartParsing(const std::string &line)
        -> tl::expected<bool, ParserError>;

    auto ParseLine(const std::string &line) -> tl::expected<void, ParserError>;

    void Reset() {
        is_parsing_ = false;
        key_ = {};
        values_ = {};
    }

private:
    void AddValuesFromLine(const std::string &line);

    // TODO(danyspin97): Make key an Optional
    std::string key_;
    std::vector<std::string> values_;
    bool is_parsing_ = false;
};

} // namespace tt

#endif // TT_ARRAY_PARSER_HPP_
