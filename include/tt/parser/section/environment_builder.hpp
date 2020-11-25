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

#ifndef TT_ENVIRONMENT_BUILDER_HPP_
#define TT_ENVIRONMENT_BUILDER_HPP_

#include <string> // for string

#include "tt/data/environment.hpp"               // for Environment
#include "tt/parser/section/section_builder.hpp" // for SectionBuilder

namespace tl {
template <typename T, typename Z> class expected;
}

namespace tt {

class Environment;

class EnvironmentBuilder : public SectionBuilder {
public:
    EnvironmentBuilder();

    [[nodiscard]] auto environment() -> Environment &&;

    auto ParseLine(const std::string &line) noexcept
        -> tl::expected<void, ParserError> override;
    auto EndParsing() noexcept -> tl::expected<void, ParserError> override;

protected:
    auto GetValidAttributes() const -> std::vector<std::string> override {
        return {};
    }
    auto GetValidArrayAttributes() const -> std::vector<std::string> override {
        return {};
    }

private:
    auto TryParseLine(const std::string &line) noexcept
        -> tl::expected<void, ParserError>;
    static auto CheckKeyIsValid(const std::string &key) noexcept
        -> tl::expected<void, ParserError>;

    Environment environment_;
};

} // namespace tt

#endif // TT_ENVIRONMENT_BUILDER_HPP_
