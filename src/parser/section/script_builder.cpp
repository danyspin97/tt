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

#include "fmt/format.h" // for format

#include "tt/parser/parser_error.hpp"      // for ParserError
#include "tt/parser/section/exception.hpp" // for CodeNotSuppliedInScriptPa...
#include "tt/parser/section/utils.hpp"     // for AttributeNotFound
#include "tt/signal.hpp"                   // for ParseSignalFromString

tt::ScriptBuilder::ScriptBuilder(const std::string &section)
    : CodeSectionBuilder(section) {}

auto tt::ScriptBuilder::EndParsing() -> tl::expected<void, ParserError> {
    if (auto ret = CodeSectionBuilder::EndParsing(); !ret.has_value()) {
        return ret;
    }

    auto ret = CreateScript();
    if (!ret.has_value()) {
        return chain_parser_error<void>(
            ret.error(), fmt::format(" in [{}] section", section()));
    }

    return {};
}

auto tt::ScriptBuilder::GetScriptAttributes()
    -> tl::expected<std::pair<Script::Type, std::string>, ParserError> {
    auto execute = GetAttribute("execute");
    if (!execute.has_value()) {
        return make_parser_error<std::pair<Script::Type, std::string>>(
            ParserError::Type::MissingExecute,
            fmt::format("'execute' is was not supplied in [{}] section",
                        section()));
    }

    auto type = GetParsedType();
    if (!type.has_value()) {
        return chain_parser_error<std::pair<Script::Type, std::string>>(
            type.error(), fmt::format(" in [{}] section", section()));
    }

    return std::make_pair(type.value(), std::move(execute.value()));
}

auto tt::ScriptBuilder::CreateScript() -> tl::expected<void, ParserError> {
    auto script_attributes = GetScriptAttributes();
    if (!script_attributes.has_value()) {
        return chain_parser_error<void>(script_attributes.error(), "");
    }
    script_ = Script(script_attributes.value().first,
                     std::move(script_attributes.value().second));
    return SetOptionalAttributeForScript(script_.value());
}

auto tt::ScriptBuilder::GetValidAttributes() const -> std::vector<std::string> {
    return {"build",       "execute",  "group",   "user",
            "down_signal", "maxdeath", "timeout", "timeout_kill"};
}

auto tt::ScriptBuilder::GetValidCodeAttributes() const
    -> std::vector<std::string> {
    return {"execute"};
}

auto tt::ScriptBuilder::GetParsedType() const
    -> tl::expected<Script::Type, ParserError> {
    auto type = GetAttribute("build");
    if (!type.has_value()) {
        return make_parser_error<Script::Type>(
            ParserError::Type::MissingScriptType, "Cound not find script type");
    }

    if (type == "execline") {
        return Script::Type::Execline;
    }
    if (type == "auto" || type == "sh") {
        return Script::Type::SH;
    }
    if (type == "bash") {
        return Script::Type::Bash;
    }
    if (type == "path") {
        return Script::Type::Path;
    }

    return make_parser_error<Script::Type>(
        ParserError::Type::InvalidScriptType,
        fmt::format("Type '{}' is not valid", type.value()));
}

namespace tt {
auto convert_int(std::function<void()> f, std::string key, std::string value)
    -> tl::expected<void, ParserError> {
    try {
        std::invoke(f);
    } catch (std::invalid_argument & /*e*/) {
        return make_parser_error<void>(
            ParserError::Type::InvalidInteger,
            fmt::format("'{}' is not a valid integer for key '{}'", value,
                        key));
    } catch (std::out_of_range & /*e*/) {
        return make_parser_error<void>(
            ParserError::Type::InvalidInteger,
            fmt::format("'{}' is not a valid value for key '{}'", value, key));
    }

    return {};
}
} // namespace tt

auto tt::ScriptBuilder::script() -> Script { return script_.value(); }

auto tt::ScriptBuilder::SetOptionalAttributeForScript(Script &script) const
    -> tl::expected<void, ParserError> {
    if (auto user = GetAttribute("user")) {
        script.user(user.value());
    }

    if (auto group = GetAttribute("group")) {
        script.group(group.value());
    }

    if (auto down_signal = GetAttribute("down_signal")) {
        auto signal = ParseSignalFromString(down_signal.value());
        if (!signal.has_value()) {
            return make_parser_error<void>(ParserError::Type::InvalidSignal,
                                           signal.error());
        }
        script.down_signal(signal.value());
    }

    if (auto max_death = GetAttribute("maxdeath")) {
        if (auto ret = convert_int(
                [&script, max_death]() {
                    script.max_death(stoi(max_death.value()));
                },
                max_death.value(), "max_death");
            !ret.has_value()) {
            return ret;
        }
    }

    if (auto timeout = GetAttribute("timeout")) {
        if (auto ret = convert_int(
                [&script, timeout]() { script.timeout(stoi(timeout.value())); },
                timeout.value(), "timeout");
            !ret.has_value()) {
            return ret;
        }
    }

    if (auto timeout_kill = GetAttribute("timeout_kill")) {
        if (auto ret = convert_int(
                [&script, timeout_kill]() {
                    script.timeout_kill(stoi(timeout_kill.value()));
                },
                timeout_kill.value(), "timeout_kill");
            !ret.has_value()) {
            return ret;
        }
    }

    return {};
}
