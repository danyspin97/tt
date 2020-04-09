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

#ifndef LIBTT_LOGGER_SCRIPT_BUILDER_HPP_
#define LIBTT_LOGGER_SCRIPT_BUILDER_HPP_

#include "libtt/data/logger_script.hpp"
#include "libtt/parser/section/script_builder.hpp"
#include <string>

namespace tt {

class LoggerScriptBuilder : public ScriptBuilder {
public:
    LoggerScriptBuilder(LoggerScript **logger_script,
                        const Environment &environment,
                        const std::string service_name)
        : ScriptBuilder(std::nullptr_t{}, environment, "log"),
          logger_script_(logger_script), service_name_(service_name) {}

    void EndParsing();

protected:
    std::string &GetAttributeForKey(const std::string key);

private:
    void CheckParsedValues();

    void SetDefaultForOptionalValues() noexcept;

    std::string GetDefaultExecute() const noexcept;

    std::string GetDefaultDestination() const noexcept;

    LoggerScript **logger_script_;

    std::string destination_;
    std::string maxsize_;
    std::string service_name_;
};

} // namespace tt

#endif // LIBTT_LOGGER_SCRIPT_BUILDER_HPP_
