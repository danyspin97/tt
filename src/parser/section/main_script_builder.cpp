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

#include <utility>

#include "tt/parser/section/main_script_builder.hpp"

#include "tt/define.h"
#include "tt/parser/section/exception.hpp"
#include "tt/parser/section/utils.hpp"
#include "tt/path/dirs.hpp"
#include "tt/utils/parse_boolean.hpp"

auto tt::MainScriptBuilder::GetAttributeForKey(const std::string &key)
    -> std::string & {
    if (key == "autostart") {
        return autostart_;
    }
    return ScriptBuilder::GetAttributeForKey(key);
}

void tt::MainScriptBuilder::SetOptionalAttributeForMainScript(
    MainScript &main_script) {
    if (!autostart_.empty()) {
        main_script.autostart(tt::utils::ParseBoolean(autostart_));
    }
}

auto tt::MainScriptBuilder::main_script() -> MainScript {
    MainScript main_script =
        MainScript(GetParsedType(), std::move(execute_move()));
    SetOptionalAttributeForScript(main_script);
    SetOptionalAttributeForMainScript(main_script);

    return main_script;
}
