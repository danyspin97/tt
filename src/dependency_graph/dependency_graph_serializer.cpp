
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
