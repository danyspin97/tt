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

#include "tt/cli/command/command.hpp"

#include <utility> // for move

#include "args.hxx" // for ValueFlag, Subparser

#include "tt/cli/define.hpp"         // for DEFAULT_VERBOSITY
#include "tt/cli/global_options.hpp" // for GlobalOptions
#include "tt/exception.hpp"          // for Exception
#include "tt/log/cli_logger.hpp"     // for CliLogger

tt::cli::Command::Command(args::Subparser &parser,
                          std::shared_ptr<GlobalOptions> global_options)
    : parser_(parser), global_options_(std::move(global_options)) {}

void tt::cli::Command::Init(const std::string &name) {
    parser_.Parse();
    auto verbosity = global_options_->verbosity().Matched()
                         ? global_options_->verbosity().Get()
                         : tt::cli::DEFAULT_VERBOSITY;
    logger_ = std::make_shared<CliLogger>(name, dirs_, verbosity,
                                          global_options_->quiet());
}

auto tt::cli::Command::Run(const std::string &name) -> int try {
    Init(name);
    return Execute();
    // all std::execption should be caught and thrown as tt::Exception with
    // a meaningful message
} catch (const tt::Exception &e) {
    logger_->LogError("{}", e.msg());
    return 255;
}

auto tt::cli::Command::logger() const -> std::shared_ptr<CliLogger> {
    return logger_;
}

auto tt::cli::Command::dirs() const -> std::shared_ptr<Dirs> { return dirs_; }
