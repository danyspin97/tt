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

#include <memory>
#include <string>

#include "spdlog/spdlog.h"
#include "tt/log/logger.hpp"

namespace tt {

class Dirs;

class CliLogger {
public:
    CliLogger(const std::string &command_name,
              const std::shared_ptr<Dirs> &dirs, const std::string &verbosity,
              bool silence_stderr);
    CliLogger() = delete;

    template <typename FormatString, typename... Args>
    void LogDebug(const FormatString &format, const Args &...args) const {
        logger_->debug(format, args...);
    }

    template <typename FormatString, typename... Args>
    void LogInfo(const FormatString &format, const Args &...args) const {
        logger_->info(format, args...);
    }

    template <typename FormatString, typename... Args>
    void LogWarn(const FormatString &format, const Args &...args) const {
        logger_->warn(format, args...);
    }

    template <typename FormatString, typename... Args>
    void LogCritical(const FormatString &format, const Args &...args) const {
        logger_->critical(format, args...);
    }

    template <typename FormatString, typename... Args>
    void LogError(const FormatString &format, const Args &...args) const {
        logger_->error(format, args...);
    }

    template <typename FormatString, typename... Args>
    void Print(const FormatString &format, const Args &...args) const {
        printer_->info(format, args...);
    }

private:
    Logger logger_;
    Logger printer_;
};

} // namespace tt
