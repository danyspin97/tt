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

#include "tt/supervision/longrun_supervisor_launcher.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <charconv>
#include <filesystem>
#include <fstream>
#include <utility>

#include "bitsery/adapter/buffer.h"
#include "bitsery/adapter/stream.h"
#include "bitsery/bitsery.h"

#include "process.hpp"

#include "tt/environment_generator.hpp"
#include "tt/exception.hpp"
#include "tt/log/longrun_logger.hpp"
#include "tt/path/dirs.hpp"
#include "tt/script/path_script_builder.hpp"
#include "tt/status.hpp"
#include "tt/utils/serialize.hpp"

tt::LongrunSupervisorLauncher::LongrunSupervisorLauncher(
    const Longrun &longrun, std::shared_ptr<Dirs> dirs)
    : longrun_(longrun), filename_(dirs->supervisedir() / longrun_.name()) {
    utils::Serialize(longrun, filename_);
}

void tt::LongrunSupervisorLauncher::Launch() {
    std::string execute{"tt supervise " + filename_.string()};
    PathScriptBuilder builder;
    tt::Environment env;
    auto command = builder.script(execute, env);
    TinyProcessLib::Process supervise{command};
}
