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

#ifndef TT_ONESHOT_OPTIONS_BUILDER_HPP_
#define TT_ONESHOT_OPTIONS_BUILDER_HPP_

#include <string> // for string

#include "tt/data/oneshot_options.hpp"           // for OneshotOptions
#include "tt/parser/section/section_builder.hpp" // for OptionsBuilder

namespace tl {
template <typename T, typename Z> class expected;
}

namespace tt {
class ArrayParser;

class OneshotOptionsBuilder : public SectionBuilder {
public:
    OneshotOptionsBuilder() noexcept;

    [[nodiscard]] auto options() noexcept -> OneshotOptions;

    [[nodiscard]] auto EndParsing() noexcept
        -> tl::expected<void, ParserError> override;

protected:
    [[nodiscard]] auto GetValidAttributes() const noexcept
        -> std::vector<std::string> override;
    [[nodiscard]] auto GetValidArrayAttributes() const noexcept
        -> std::vector<std::string> override;

private:
    OneshotOptions options_;
};

} // namespace tt

#endif // TT_ONESHOT_OPTIONS_BUILDER_HPP_
