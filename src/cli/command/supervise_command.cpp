/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include "tt/cli/command/supervise_command.hpp"

#include <poll.h>
#include <sys/stat.h>
#include <unistd.h>

#include <array>
#include <filesystem>
#include <utility>

#include "args.hxx"
#include "spdlog/spdlog.h"

#include "tt/cli/global_options.hpp"

tt::cli::SuperviseCommand::SuperviseCommand(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    : Command(parser, std::move(common_options)),
      script_(parser, "script", "script arguments to execute") {}

auto tt::cli::SuperviseCommand::Dispatch(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    -> int {
    SuperviseCommand command =
        SuperviseCommand(parser, std::move(common_options));
    return command.InitAndExecute();
}

auto tt::cli::SuperviseCommand::Execute() -> int {
    // SpawnLongrunScript();

    return 0;
}
