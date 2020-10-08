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

#include "tt/cli/command/supervise_command.hpp"

#include <poll.h>
#include <sys/stat.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <utility>

#include "args.hxx"

#include "spdlog/spdlog.h"

#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"

#include "tt/cli/global_options.hpp"
#include "tt/svc/supervise_longrun.hpp"

tt::cli::SuperviseCommand::SuperviseCommand(
    args::Subparser &parser, std::shared_ptr<GlobalOptions> common_options)
    : Command(parser, std::move(common_options)),
      filename_(parser, "filename", "Filename to read the longrun from") {}

auto tt::cli::SuperviseCommand::Execute() -> int {
    Longrun longrun;
    auto buffer = GetLongrunBuffer();

    auto state = bitsery::quickDeserialization<
        bitsery::InputBufferAdapter<std::vector<uint8_t>>>(
        {buffer.begin(), buffer.size()}, longrun);

    if (state.first != bitsery::ReaderError::NoError || !state.second) {
        spdlog::error("Could not deserialize longrun in file {}",
                      args::get(filename_));
        return 255;
    }

    auto supervise = SuperviseLongrun{std::move(longrun)};
    supervise.Spawn();
    return 0;
}

auto tt::cli::SuperviseCommand::GetLongrunBuffer() -> std::vector<uint8_t> {
    auto filename = args::get(filename_);
    if (!std::filesystem::exists(filename)) {
        spdlog::error("File {} does not exist", filename);
        // TODO: Exit
    }

    std::ifstream file{filename, std::ifstream::binary | std::ifstream::in};
    if (!file.is_open()) {
        spdlog::error("Cannot open file {} for reading", filename);
        // TODO: Exit
    }
    // Properly read binary file
    file.unsetf(std::ios::skipws);

    // Get buffer size
    file.seekg(0, std::ios::end);
    auto buffer_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer;
    buffer.reserve(buffer_size);

    std::istream_iterator<uint8_t> start(file);
    std::istream_iterator<uint8_t> end;
    buffer.insert(buffer.begin(), start, end);

    return buffer;
}
