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

#include "tt/cli/log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

void tt::cli::setupConsoleLoggers(const std::string &verbosity,
                                  bool silence_stderr) {
    spdlog::drop_all();

    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    console->set_level(spdlog::level::from_str(verbosity));
    if (silence_stderr) {
        console->set_level(spdlog::level::off);
    }

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(console);
    auto console_sink =
        std::make_shared<spdlog::logger>("console", begin(sinks), end(sinks));
    spdlog::set_default_logger(console_sink);
}
