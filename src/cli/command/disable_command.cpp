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

#include "tt/cli/command/disable_command.hpp"

#include "args.hxx"

#include "tt/cli/global_options.hpp"
#include "tt/exception.hpp" // for Exception
#include "tt/file_lock.hpp" // for FileLock

tt::cli::DisableCommand::DisableCommand(args::Subparser &parser)
    : Command(parser), services_(parser, "service", "services to disable") {}

auto tt::cli::DisableCommand::Execute() -> int {
    FileLock lock(dirs()->livedir() / ".graph_lock");
    if (!lock.TryLock()) {
        throw Exception(
            "Another instance of tt-enable or tt-disable is running");
    }
    return 0;
}
