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

#include <utility>

#include "tt/parser/section/logger_script_builder.hpp"

#include "tt/define.h"
#include "tt/dirs.hpp"
#include "tt/parser/section/exception.hpp"

using std::string;

using tt::kDefaultLogGroup;
using tt::kDefaultLogUser;
using tt::LoggerScript;
using tt::LoggerScriptBuilder;
using tt::LoggerScriptInvalidSettingsException;

LoggerScriptBuilder::LoggerScriptBuilder(std::string service_name)
    : ScriptBuilder("log"), service_name_(std::move(service_name)) {}

void LoggerScriptBuilder::EndParsing() {
    CheckParsedValues();
    SetDefaultForOptionalValues();
}

void LoggerScriptBuilder::CheckParsedValues() {
    bool is_execute_set = !execute().empty();
    bool is_type_set = !type().empty();
    // A XOR B
    if (is_execute_set != is_type_set) {
        const auto *msg = "Both execute and type attributes needs to set";
        throw LoggerScriptInvalidSettingsException(msg);
    }

    auto is_destination_set = !destination_.empty();
    auto is_maxsize_set = !maxsize_.empty();
    if ((is_execute_set || is_type_set) &&
        (is_destination_set || is_maxsize_set)) {
        const auto *msg =
            "destination and maxsize attributes cannot be set when "
            "setting execute";
        throw LoggerScriptInvalidSettingsException(msg);
    }
}

auto LoggerScriptBuilder::GetAttributeForKey(const string &key) -> string & {
    if (key == "destination") {
        return destination_;
    }
    if (key == "maxsize") {
        return maxsize_;
    }
    return ScriptBuilder::GetAttributeForKey(key);
}

void LoggerScriptBuilder::SetDefaultForOptionalValues() noexcept {
    if (type().empty()) {
        type("auto");
    }

    if (maxsize_.empty()) {
        maxsize_ = "8000000"; // 8MB
    }

    if (destination_.empty()) {
        destination_ = GetDefaultDestination();
    }

    if (execute().empty()) {
        execute(GetDefaultExecute());
    }

    if (user().empty()) {
        user(kDefaultLogUser);
    }

    if (group().empty()) {
        group(kDefaultLogGroup);
    }
}

auto LoggerScriptBuilder::GetDefaultExecute() const noexcept -> std::string {
    return "exec -c s6-log -- s" + maxsize_ + " n20 t " + destination_;
}

auto LoggerScriptBuilder::GetDefaultDestination() const noexcept
    -> std::string {
    tt::Dirs &dirs = tt::Dirs::GetInstance();
    return string{dirs.logdir()} + "/" + service_name_;
}

auto LoggerScriptBuilder::logger_script() -> LoggerScript {
    LoggerScript logger_script =
        LoggerScript(GetParsedType(), std::move(execute_move()), service_name_,
                     std::move(user_move()), std::move(group_move()));
    // TODO: Check if EndParsing has been called
    return logger_script;
}
