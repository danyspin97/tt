/*
 *
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

#include <string>  // for string
#include <utility> // for move

#include "fmt/format.h" // for format

#include "tt/data/long_lived_script.hpp"                   // for LongLived...
#include "tt/parser/parser_error.hpp"                      // for ParserError
#include "tt/parser/section/long_lived_script_builder.hpp" // for LongLived...
#include "tt/parser/section/main_script_builder.hpp"       // for MainScrip...

auto tt::LongLivedScriptBuilder::GetValidAttributes() const noexcept
    -> std::vector<std::string> {
    auto valid_attributes = MainScriptBuilder::GetValidAttributes();
    valid_attributes.emplace_back("notify");
    return valid_attributes;
}

auto tt::LongLivedScriptBuilder::SetOptionalAttributeForLongLivedScript(
    LongLivedScript &long_lived_script) noexcept
    -> tl::expected<void, ParserError> {
    if (auto notify = GetAttribute("notify")) {
        try {
            long_lived_script.notify(stoi(notify.value()));
        } catch (std::invalid_argument &) {
            return make_parser_error<void>(
                ParserError::Type::InvalidInteger,
                fmt::format("'{}' is not a valid integer", notify.value()));
        }
    }

    return SetOptionalAttributeForMainScript(long_lived_script);
}

auto tt::LongLivedScriptBuilder::CreateScript() noexcept
    -> tl::expected<void, ParserError> {
    auto script_attributes = GetScriptAttributes();
    if (!script_attributes.has_value()) {
        return tl::unexpected(script_attributes.error());
    }
    long_lived_script_ =
        LongLivedScript(script_attributes.value().first,
                        std::move(script_attributes.value().second));
    return SetOptionalAttributeForLongLivedScript(long_lived_script_.value());
}

auto tt::LongLivedScriptBuilder::long_lived_script() const noexcept
    -> LongLivedScript {
    return long_lived_script_.value();
}

auto tt::LongLivedScriptBuilder::long_lived_script() noexcept
    -> LongLivedScript && {
    return std::move(long_lived_script_.value());
}
