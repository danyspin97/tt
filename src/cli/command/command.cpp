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

#include <unistd.h> // for geteuid

#include "args.hxx" // for ValueFlag, Subparser

#include "tt/cli/define.hpp"         // for DEFAULT_VERBOSITY
#include "tt/cli/global_options.hpp" // for GlobalOptions
#include "tt/log/cli_logger.hpp"     // for CliLogger
#include "tt/path/system_dirs.hpp"   // for SystemDirs
#include "tt/path/user_dirs.hpp"     // for UserDirs

tt::cli::Command::Command(args::Subparser &parser,
                          std::shared_ptr<GlobalOptions> global_options)
    : parser_(parser), global_options_(std::move(global_options)),
      dirs_(geteuid() > 0 ? std::make_shared<UserDirs>()
                          : std::make_shared<SystemDirs>()) {}

auto tt::cli::Command::InitAndExecute() -> int {
    parser_.Parse();
    auto verbosity = global_options_->verbosity().Matched()
                         ? global_options_->verbosity().Get()
                         : tt::cli::DEFAULT_VERBOSITY;
    logger_ =
        std::make_shared<CliLogger>(dirs_, verbosity, global_options_->quiet());
    return Execute();
}

auto tt::cli::Command::logger() const -> std::shared_ptr<CliLogger> {
    return logger_;
}

auto tt::cli::Command::dirs() const -> std::shared_ptr<Dirs> { return dirs_; }
