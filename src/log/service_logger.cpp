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

#include "tt/log/script_logger.hpp" // for ScriptLogger
#include "tt/path/dirs.hpp"         // for Dirs

tt::ServiceLogger::ServiceLogger(std::string service_name,
                                 const std::shared_ptr<Dirs> &dirs)
    : service_name_(std::move(service_name)) {
    // Check if the logger has already been added
    if ((logger_ = spdlog::get(service_name_))) {
        return;
    }

    auto service_log_path =
        dirs->logdir() /
        std::filesystem::path{service_name_ + std::string{".log"}};
    logger_ = spdlog::basic_logger_mt<spdlog::async_factory>(service_name_,
                                                             service_log_path);

    logger_->set_pattern("{%d:%m:%Y %T} %v");
}

auto tt::ServiceLogger::GetScriptLogger() const -> ScriptLogger {
    return ScriptLogger(logger_);
}

auto tt::ServiceLogger::logger() const -> Logger { return logger_; }

auto tt::ServiceLogger::service_name() const -> const std::string & {
    return service_name_;
}
