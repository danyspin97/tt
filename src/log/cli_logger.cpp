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

#include "tt/log/cli_logger.hpp"

#include <filesystem>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "tt/define.h"
#include "tt/dirs.hpp"
#include "tt/log/cli_logger.hpp"
#include "tt/status.hpp"

tt::CliLogger::CliLogger(const std::string &verbosity, bool silence_stderr) {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    console->set_level(spdlog::level::from_str(verbosity));
    if (silence_stderr) {
        console->set_level(spdlog::level::off);
    }

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(console);

    std::filesystem::path logdir = Status::GetInstance().dirs().logdir();

    if (isatty(STDOUT_FILENO) == 0) {
        auto cli_file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(logdir /
                                                                "cli.log");
        sinks.push_back(cli_file_sink);
    }

    auto cli_sinks =
        std::make_shared<spdlog::logger>("cli", begin(sinks), end(sinks));
    spdlog::set_default_logger(cli_sinks);

    auto status_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
        kServiceStatusLog, logdir / kServiceStatusLogFile);
}
