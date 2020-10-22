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
#pragma once

#include <filesystem>

#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"

#include "tt/exception.hpp"
#include "tt/utils/read_buffer_from_file.hpp"

namespace tt::utils {

template <typename T> auto Deserialize(const std::filesystem::path &path) -> T {
    T obj;
    auto buffer = ReadBufferFromFile(path);
    auto state = bitsery::quickDeserialization<
        bitsery::InputBufferAdapter<std::vector<uint8_t>>>(
        {buffer.begin(), buffer.size()}, obj);
    if (state.first == bitsery::ReaderError::NoError && state.second) {
        return obj;
    }
    throw tt::Exception("Error reading from serialized data");
}

} // namespace tt::utils
