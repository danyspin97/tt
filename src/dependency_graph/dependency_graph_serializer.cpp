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

#include "tt/dependency_graph/dependency_graph_serializer.hpp"

#include <fstream>

#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"
#include "bitsery/traits/string.h"
#include "bitsery/traits/vector.h"

#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/dirs.hpp"
#include "tt/exception.hpp"

std::tuple<tt::DependencyGraphSerializer::Buffer, size_t>
tt::DependencyGraphSerializer::Serialize(tt::DependencyGraph graph) {
    Buffer buffer;
    size_t written_size =
        bitsery::quickSerialization<bitsery::OutputBufferAdapter<Buffer>>(
            buffer, graph);

    return std::make_tuple(buffer, written_size);
}

tt::DependencyGraph
tt::DependencyGraphSerializer::Deserialize(const Buffer &buffer,
                                           size_t written_size) {
    tt::DependencyGraph graph;
    auto state =
        bitsery::quickDeserialization<bitsery::InputBufferAdapter<Buffer>>(
            {buffer.begin(), written_size}, graph);

    if (state.first == bitsery::ReaderError::NoError && state.second) {
        return graph;
    }

    throw tt::Exception("Error reading from serialized data");
}
