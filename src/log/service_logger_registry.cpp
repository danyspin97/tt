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

#include "tt/log/service_logger_registry.hpp"

#include <filesystem> // for operator/, path
#include <utility>    // for move

#include "spdlog/async.h"                 // for async_factory
#include "spdlog/logger.h"                // for logger
#include "spdlog/sinks/basic_file_sink.h" // for basic_logger_mt
#include "spdlog/spdlog.h"                // for get

#include "tt/define.h"                 // for kServiceStatusLog, kSer...
#include "tt/log/longrun_logger.hpp"   // for LongrunLogger
#include "tt/log/oneshot_logger.hpp"   // for OneshotLogger
#include "tt/path/directory_names.hpp" // for kServiceLogDirectoryName
#include "tt/path/dirs.hpp"            // for Dirs
#include "tt/svc/service_status.hpp" // for ServiceStatus, GetServiceStatusName

tt::ServiceLoggerRegistry::ServiceLoggerRegistry(std::shared_ptr<Dirs> dirs)
    : dirs_(std::move(dirs)) {
    logger_ = spdlog::basic_logger_mt<spdlog::async_factory>(
        kServiceStatusLog, dirs_->logdir() / kServiceStatusLogFile);
}

tt::ServiceLoggerRegistry::~ServiceLoggerRegistry() { logger_->flush(); }

auto tt::ServiceLoggerRegistry::GetLongrunLogger(const std::string &name)
    -> LongrunLogger {
    return LongrunLogger{name, logger_, GetScriptLoggerForService(name)};
}

auto tt::ServiceLoggerRegistry::GetOneshotLogger(const std::string &name)
    -> OneshotLogger {
    return OneshotLogger{name, logger_, GetScriptLoggerForService(name)};
}

auto tt::ServiceLoggerRegistry::GetScriptLoggerForService(
    const std::string &service_name) -> Logger {
    // Check if the logger has already been added
    Logger logger;
    if ((logger = spdlog::get(service_name))) {
        return logger;
    }

    // Since there is only one script running for each service
    // there cannot be two different threads running this method for the
    // same service
    auto service_log_path =
        dirs_->logdir() / kServiceLogDirectoryName /
        std::filesystem::path{service_name + std::string{".log"}};
    logger = spdlog::basic_logger_mt<spdlog::async_factory>(service_name,
                                                            service_log_path);

    logger->set_pattern("{%d:%m:%Y %T} %v");

    return logger;
}

void tt::ServiceLoggerRegistry::LogServiceStatus(
    const std::string &service_name, tt::ServiceStatus status) const {
    logger_->info("{} is {}", service_name, GetServiceStatusName(status));
}

void tt::ServiceLoggerRegistry::Flush() const { logger_->flush(); }
