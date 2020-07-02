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

ScriptBuilder::ScriptBuilder(const std::string &section)
    : CodeSectionBuilder(section) {}

void ScriptBuilder::EndParsing() {
    if (execute_.empty()) {
        const auto error_message =
            "Code was not supplied in section [" + section_ + "]";
        throw tt::CodeNotSuppliedInScriptParserException(error_message);
    }

    finished_ = true;
}

auto ScriptBuilder::GetAttributeForKey(const string &key) -> string & {
    if (key == "build") {
        return type_;
    }
    if (key == "execute") {
        return execute_;
    }
    if (key == "group") {
        return group_;
    }
    if (key == "user") {
        return user_;
    }

    AttributeNotFound(key, section_);
    // Never reached
    return section_;
}

auto ScriptBuilder::GetCodeAttributeForKey(const string &key) -> string & {
    if (key == "execute") {
        return execute_;
    }

    AttributeNotFound(key, section_);
    // Never reached
    return section_;
}

auto tt::ScriptBuilder::GetParsedType() const -> Script::Type {
    // TODO: add "path" type
    if (type_ == "auto" || type_ == "execline") {
        return Script::Type::Execline;
    }
    if (type_ == "bash") {
        return Script::Type::Bash;
    }

    auto error_message = "Type " + type_ + " is not allowed.";
    throw tt::ScriptTypeNotValidException(error_message);
}

auto ScriptBuilder::script() const -> Script {
    Script script = Script(GetParsedType(), execute_);

    if (!user_.empty()) {
        script.user(user_);
    }

    if (!group_.empty()) {
        script.group(group_);
    }
    return script;
}

auto ScriptBuilder::HasScript() const -> bool { return finished_; }
