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

#include "tt/utils/read_buffer_from_file.hpp"

#include <fstream>

#include "spdlog/spdlog.h"

auto tt::utils::ReadBufferFromFile(const std::filesystem::path &filename)
    -> std::vector<uint8_t> {
    if (!std::filesystem::exists(filename)) {
        spdlog::error("File {} does not exist", filename.c_str());
        // TODO: throw exception
    }

    std::ifstream file{filename, std::ifstream::binary | std::ifstream::in};
    if (!file.is_open()) {
        spdlog::error("Cannot open file {} for reading", filename.c_str());
        // TODO: throw exception
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
