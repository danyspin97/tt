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

#include "tt/parser/section/logger_script_builder.hpp"

#include "tt/define.h"
#include "tt/dirs.h"
#include "tt/parser/section/exception.hpp"

using std::string;

using tt::dirs;
using tt::kDefaultLogGroup;
using tt::kDefaultLogUser;
using tt::LoggerScript;
using tt::LoggerScriptBuilder;
using tt::LoggerScriptInvalidSettingsException;
using tt::Script;

LoggerScriptBuilder::LoggerScriptBuilder(Environment &environment,
                                         const std::string &service_name)
    : ScriptBuilder(environment, "log"), service_name_(service_name) {}

void LoggerScriptBuilder::EndParsing() {
    CheckParsedValues();
    SetDefaultForOptionalValues();
}

void LoggerScriptBuilder::CheckParsedValues() {
    bool is_execute_set = !execute_.empty();
    bool is_type_set = !type_.empty();
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

string &LoggerScriptBuilder::GetAttributeForKey(const string &key) {
    if (key == "destination") {
        return destination_;
    }
    if (key == "maxsize") {
        return maxsize_;
    }
    return ScriptBuilder::GetAttributeForKey(key);
}

void LoggerScriptBuilder::SetDefaultForOptionalValues() noexcept {
    if (type_.empty()) {
        type_ = "auto";
    }

    if (maxsize_.empty()) {
        maxsize_ = "8000000"; // 8MB
    }

    if (destination_.empty()) {
        destination_ = GetDefaultDestination();
    }

    if (execute_.empty()) {
        execute_ = GetDefaultExecute();
    }

    if (user_.empty()) {
        user_ = kDefaultLogUser;
    }

    if (group_.empty()) {
        group_ = kDefaultLogGroup;
    }
}

std::string LoggerScriptBuilder::GetDefaultExecute() const noexcept {
    return "exec -c s6-log -- s" + maxsize_ + " n20 t " + destination_;
}

std::string LoggerScriptBuilder::GetDefaultDestination() const noexcept {
    return string{dirs.log} + "/" + service_name_;
}

LoggerScript LoggerScriptBuilder::logger_script() const {
    LoggerScript logger_script = LoggerScript(
        GetParsedType(), execute_, environment_, service_name_, user_, group_);
    // TODO: Check if EndParsing has been called
    return logger_script;
}
