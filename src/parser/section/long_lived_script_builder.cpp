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

#include "tt/parser/section/long_lived_script_builder.hpp"

#include "tt/data/long_lived_script.hpp"
#include "tt/define.h"
#include "tt/parser/section/exception.hpp"
#include "tt/path/dirs.hpp"

auto tt::LongLivedScriptBuilder::GetAttributeForKey(const std::string &key)
    -> std::string & {
    if (key == "notify") {
        return notify_;
    }
    return MainScriptBuilder::GetAttributeForKey(key);
}

void tt::LongLivedScriptBuilder::SetOptionalAttributeForLongLivedScript(
    LongLivedScript &long_lived_script) {
    if (!notify_.empty()) {
        long_lived_script.notify(stoi(notify_));
    }
}

auto tt::LongLivedScriptBuilder::long_lived_script() -> LongLivedScript {
    LongLivedScript long_lived_script =
        LongLivedScript(GetParsedType(), std::move(execute_move()));
    SetOptionalAttributeForScript(long_lived_script);
    SetOptionalAttributeForMainScript(long_lived_script);
    SetOptionalAttributeForLongLivedScript(long_lived_script);

    return long_lived_script;
}
