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

#include <string>  // for string, operator==
#include <utility> // for move

#include "fmt/format.h" // for format

#include "tt/data/main_script.hpp"                   // for MainScript
#include "tt/parser/parser_error.hpp"                // for ParserError
#include "tt/parser/section/main_script_builder.hpp" // for MainScriptBuilder
#include "tt/parser/section/script_builder.hpp"      // for ScriptBuilder
#include "tt/utils/parse_boolean.hpp"                // for ParseBoolean

auto tt::MainScriptBuilder::GetValidAttributes() const
    -> std::vector<std::string> {
    auto valid_attributes = ScriptBuilder::GetValidAttributes();
    valid_attributes.emplace_back("autostart");
    return valid_attributes;
}

auto tt::MainScriptBuilder::SetOptionalAttributeForMainScript(
    MainScript &main_script) -> tl::expected<void, ParserError> const {
    auto autostart = GetAttribute("autostart");
    if (autostart.has_value()) {
        auto ret = utils::ParseBoolean(autostart.value());
        if (!ret.has_value()) {
            return make_parser_error<void>(ParserError::Type::InvalidBoolean,
                                           ret.error());
        }
        main_script.autostart(ret.value());
    }

    return SetOptionalAttributeForScript(main_script);
}

auto tt::MainScriptBuilder::CreateScript() -> tl::expected<void, ParserError> {
    auto script_attributes = GetScriptAttributes();
    if (!script_attributes.has_value()) {
        return tl::unexpected(script_attributes.error());
    }
    main_script_ = MainScript(script_attributes.value().first,
                              std::move(script_attributes.value().second));
    return SetOptionalAttributeForMainScript(main_script_.value());
}

auto tt::MainScriptBuilder::main_script() -> MainScript {
    return main_script_.value();
}
