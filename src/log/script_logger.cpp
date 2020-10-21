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

#include "tt/log/script_logger.hpp"

#include "fmt/format.h"

#include "spdlog/spdlog.h"

tt::ScriptLogger::ScriptLogger(Logger logger) : logger_(std::move(logger)) {}

void tt::ScriptLogger::Log(Type type, const char *bytes, size_t size) {
    bool is_stdout = type == Type::STDOUT;
    std::string &last_line = is_stdout ? stdout_line_ : stderr_line_;
    std::string output{bytes, size};
    size_t index = output.find('\n');
    size_t last_index = 0;

    // If there is output remaining from the latest Read call
    // Append to it and log
    if (index != std::string::npos && !last_line.empty()) {
        // Lock is not needed because this function is called synchronously
        // from a thread in Process
        size_t count = index - last_index;
        if (count != 0) {
            fmt::format_to(std::back_inserter(last_line), "{}",
                           output.substr(last_index, count));
        }

        logger_->info("[{}] {}", is_stdout ? "stdout" : "stderr", last_line);
        last_line.clear();
        last_index = index + 1;
        index = output.find('\n', last_index);
    }

    // Parse all the remaining output
    while (index != std::string::npos) {
        logger_->info("[{}] {}", is_stdout ? "stdout" : "stderr",
                      output.substr(last_index, index - last_index));
        last_index = index + 1;
        index = output.find('\n', last_index);
    }

    if (last_index != index) {
        fmt::format_to(std::back_inserter(last_line), "{}",
                       output.substr(last_index, size - last_index));
    }
}
