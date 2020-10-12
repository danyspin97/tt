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

#include <fstream>

#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"

#include "fmt/format.h"

#include "tt/exception.hpp"

namespace tt::utils {

template <typename T> void Serialize(T obj, const std::filesystem::path &path) {
    std::fstream file{path, std::fstream::binary | std::fstream::trunc |
                                std::fstream::out};
    if (!file.is_open()) {
        throw tt::Exception(
            fmt::format("Cannot open file {} for writing", path.c_str()));
    }
    std::ostream_iterator<uint8_t> file_iterator(file);

    using Buffer = std::vector<uint8_t>;
    Buffer buffer;

    auto written_size =
        bitsery::quickSerialization<bitsery::OutputBufferAdapter<Buffer>>(
            buffer, obj);
    auto end = buffer.begin();
    std::advance(end, written_size);
    std::copy(buffer.begin(), end, file_iterator);
}

} // namespace tt::utils
