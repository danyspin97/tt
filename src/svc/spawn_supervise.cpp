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

#include "tt/svc/spawn_supervise.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <charconv>
#include <filesystem>
#include <fstream>
#include <utility>

#include "spdlog/spdlog.h"

#include "bitsery/adapter/stream.h"
#include "bitsery/bitsery.h"
#include <bitsery/adapter/buffer.h>

#include "tt/dirs.hpp"
#include "tt/exception.hpp"

tt::SpawnSupervise::SpawnSupervise(const Longrun &longrun,
                                   std::vector<const char *> environment)
    : longrun_(longrun), environment_(std::move(environment)) {
    auto filename = GetScriptFilename();
    WriteScriptToFile(filename);
    Spawn(filename);
}

void tt::SpawnSupervise::Spawn(const std::string &filename) {
    if (int pid = fork(); pid == 0) {
        execve("tt", const_cast<char **>(GetExecArgs(filename).data()),
               const_cast<char *const *>(environment_.data()));
        spdlog::critical("An error had happened while running execve: {}",
                         strerror(errno));
        std::exit(1);
    }
}

void tt::SpawnSupervise::WriteScriptToFile(const std::string &filename) {
    std::fstream file{filename, std::fstream::binary | std::fstream::trunc | std::fstream::out};
    if (!file.is_open()) {
        spdlog::error("Error opening file {}", filename);
        throw tt::Exception("");
    }

    using Buffer = std::array<uint8_t, 10000>;
    using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
    Buffer buffer{};
    auto writtenSize =
        bitsery::quickSerialization<OutputAdapter>(buffer, longrun_);

    file.write(reinterpret_cast<char *>(buffer.data()), writtenSize);
    file.flush();
    file.close();
}

auto tt::SpawnSupervise::GetScriptFilename() -> std::string {
    auto &dirs = Dirs::GetInstance();
    std::filesystem::path filename(dirs.livedir());
    filename /= "supervise";
    filename /= longrun_.name();
    return filename;
}

auto tt::SpawnSupervise::GetExecArgs(const std::string &filename)
    -> std::vector<char *> {
    std::vector<char *> args{};
    args.push_back(const_cast<char *>("tt"));
    args.push_back(const_cast<char *>("supervise"));
    args.push_back(const_cast<char *>(filename.data()));
    args.push_back(0);
    return args;
}
