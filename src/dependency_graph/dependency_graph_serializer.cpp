
#include "tt/dependency_graph/dependency_graph_serializer.hpp"

#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"
#include "bitsery/traits/vector.h"

#include "tt/dependency_graph/dependency_graph.hpp"
#include "tt/exception.hpp"

std::tuple<tt::DependencyGraphSerializer::Buffer, size_t>
tt::DependencyGraphSerializer::Serialize(const tt::DependencyGraph &graph) {
    using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;

    Buffer buffer;
    auto writtenSize =
        bitsery::quickSerialization<OutputAdapter>(buffer, graph);
    return make_tuple(buffer, writtenSize);
}

tt::DependencyGraph tt::DependencyGraphSerializer::Deserialize(
    const tt::DependencyGraphSerializer::Buffer &buffer, size_t writtenSize) {
    using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

    tt::DependencyGraph graph;
    auto state = bitsery::quickDeserialization<InputAdapter>(
        {buffer.begin(), writtenSize}, buffer);

    if (state.first == bitsery::ReaderError::NoError && state.second) {
        return graph;
    }

    throw tt::Exception("Error reading from serialized data");
}
