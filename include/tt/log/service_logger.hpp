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

#pragma once

#include <memory> // for shared_ptr
#include <string> // for string

#include "tt/log/logger.hpp" // for Logger

namespace tt {

class Dirs;
class ScriptLogger;

class ServiceLogger {
public:
    ServiceLogger(std::string service_name, Logger status_logger,
                  Logger script_logger);
    virtual ~ServiceLogger() = default;

    // Returns a new instance of ScriptLogger
    [[nodiscard]] auto GetScriptLogger() const -> ScriptLogger;

    // Log messages to logger_
    virtual void Start() const = 0;
    virtual void Success() const = 0;
    virtual void Fail() const = 0;

protected:
    [[nodiscard]] auto status_logger() const -> Logger;
    [[nodiscard]] auto service_name() const -> const std::string &;

private:
    std::string service_name_;
    Logger status_logger_;
    Logger script_logger_;
};

} // namespace tt
