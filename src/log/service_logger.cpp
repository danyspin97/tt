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

#include "tt/log/service_logger.hpp"

#include <filesystem> // for path, operator/
#include <utility>    // for move

#include "spdlog/async.h"                 // for async_factory
#include "spdlog/logger.h"                // for logger
#include "spdlog/sinks/basic_file_sink.h" // for basic_logger_mt
#include "spdlog/spdlog.h"                // for get

#include "tt/log/script_logger.hpp"    // for ScriptLogger
#include "tt/path/directory_names.hpp" // for kServiceLogDirectoryName
#include "tt/path/dirs.hpp"            // for Dirs

tt::ServiceLogger::ServiceLogger(std::string service_name, Logger status_logger,
                                 Logger script_logger)
    : service_name_(std::move(service_name)),
      status_logger_(std::move(status_logger)),
      script_logger_(std::move(script_logger)) {}

auto tt::ServiceLogger::GetScriptLogger() const -> ScriptLogger {
    return ScriptLogger(script_logger_);
}

auto tt::ServiceLogger::status_logger() const -> Logger {
    return status_logger_;
}

auto tt::ServiceLogger::service_name() const -> const std::string & {
    return service_name_;
}
