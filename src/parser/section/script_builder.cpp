/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include "tt/parser/section/script_builder.hpp"

#include <string>

#include "tt/parser/section/exception.hpp"
#include "tt/parser/section/section_builder.hpp"
#include "tt/parser/section/utils.hpp"

using std::string;

using tt::AttributeNotFound;
using tt::Script;
using tt::ScriptBuilder;

ScriptBuilder::ScriptBuilder(const Environment &environment,
                             const std::string section)
    : CodeSectionBuilder(section), environment_(environment) {}

void ScriptBuilder::EndParsing() {
    if (execute_ == "") {
        const auto error_message =
            "Code was not supplied in section [" + section_ + "]";
        throw tt::CodeNotSuppliedInScriptParserException(error_message);
    }
}

string &ScriptBuilder::GetAttributeForKey(const string key) {
    if (key == "build") {
        return type_;
    } else if (key == "execute") {
        return execute_;
    } else if (key == "group") {
        return group_;
    } else if (key == "user") {
        return user_;
    }

    AttributeNotFound(key, section_);
    // Never reached
    return section_;
}

string &ScriptBuilder::GetCodeAttributeForKey(const string key) {
    if (key == "execute") {
        return execute_;
    }

    AttributeNotFound(key, section_);
    // Never reached
    return section_;
}

Script::Type tt::ScriptBuilder::GetParsedType() const {
    // TODO: add "path" type
    if (type_ == "auto" || type_ == "execline") {
        return Script::Type::Execline;
    } else if (type_ == "bash") {
        return Script::Type::Bash;
    }

    auto error_message = "Type " + type_ + " is not allowed.";
    throw tt::ScriptTypeNotValidException(error_message);
}

Script ScriptBuilder::script() const {
    Script script = Script(GetParsedType(), execute_, environment_);

    if (user_ != "") {
        script.user(user_);
    }

    if (group_ != "") {
        script.group(group_);
    }
    // TODO: Check if EndParsing has been called
    return script;
}