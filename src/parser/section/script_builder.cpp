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

#include "tt/parser/section/script_builder.hpp"

#include <string>  // for string, operator==, alloc...
#include <utility> // for move

#include "tt/parser/section/exception.hpp" // for CodeNotSuppliedInScriptPa...
#include "tt/parser/section/utils.hpp"     // for AttributeNotFound
#include "tt/signal.hpp"                   // for ParseSignalFromString

tt::ScriptBuilder::ScriptBuilder(const std::string &section)
    : CodeSectionBuilder(section) {}

void tt::ScriptBuilder::EndParsing() {
    if (execute_.empty()) {
        const auto error_message =
            "Code was not supplied in section [" + section_ + "]";
        throw tt::CodeNotSuppliedInScriptParserException(error_message);
    }

    finished_ = true;
}

auto tt::ScriptBuilder::GetAttributeForKey(const std::string &key)
    -> std::string & {
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
    if (key == "down_signal") {
        return down_signal_;
    }
    if (key == "maxdeath") {
        return max_death_;
    }
    if (key == "timeout") {
        return timeout_;
    }
    if (key == "timeout_kill") {
        return timeout_kill_;
    }

    AttributeNotFound(key, section_);
}
auto tt::ScriptBuilder::GetCodeAttributeForKey(const std::string &key)
    -> std::string & {
    if (key == "execute") {
        return execute_;
    }

    AttributeNotFound(key, section_);
}

auto tt::ScriptBuilder::GetParsedType() const -> Script::Type {
    // TODO: add "path" type
    if (type_ == "execline") {
        return Script::Type::Execline;
    }
    if (type_ == "auto" || type_ == "sh") {
        return Script::Type::SH;
    }
    if (type_ == "bash") {
        return Script::Type::Bash;
    }
    if (type_ == "path") {
        return Script::Type::Path;
    }

    auto error_message = "Type " + type_ + " is not allowed.";
    throw tt::ScriptTypeNotValidException(error_message);
}

auto tt::ScriptBuilder::script() -> Script {
    Script script = Script(GetParsedType(), std::move(execute_));
    SetOptionalAttributeForScript(script);
    return script;
}

void tt::ScriptBuilder::SetOptionalAttributeForScript(Script &script) const {
    if (!user_.empty()) {
        script.user(user_);
    }

    if (!group_.empty()) {
        script.group(group_);
    }

    if (!down_signal_.empty()) {
        script.down_signal(ParseSignalFromString(down_signal_));
    }

    if (!max_death_.empty()) {
        script.max_death(stoi(max_death_));
    }

    if (!timeout_.empty()) {
        script.timeout(stoi(timeout_));
    }

    if (!timeout_kill_.empty()) {
        script.timeout_kill(stoi(timeout_kill_));
    }
}

auto tt::ScriptBuilder::HasScript() const -> bool { return finished_; }

auto tt::ScriptBuilder::execute() const -> const std::string & {
    return execute_;
}

void tt::ScriptBuilder::execute(std::string &&execute) {
    execute_ = std::move(execute);
}

auto tt::ScriptBuilder::execute() -> std::string && {
    return std::move(execute_);
}

auto tt::ScriptBuilder::type() const -> const std::string & { return type_; }

void tt::ScriptBuilder::type(const std::string &type) { type_ = type; }

auto tt::ScriptBuilder::user() const -> const std::string & { return user_; }

void tt::ScriptBuilder::user(const std::string &user) { user_ = user; }

auto tt::ScriptBuilder::user() -> std::string && { return std::move(user_); }

auto tt::ScriptBuilder::group() const -> const std::string & { return group_; }

void tt::ScriptBuilder::group(const std::string &group) { group_ = group; }

auto tt::ScriptBuilder::group() -> std::string && { return std::move(group_); }
