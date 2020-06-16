
#pragma once

#include <cstdint>
#include <vector>

namespace tt {

class DependencyGraph;

class DependencyGraphSerializer {
public:
    using Buffer = std::vector<uint8_t>;
    static std::tuple<Buffer, size_t> Serialize(const DependencyGraph &graph);
    static DependencyGraph Deserialize(const Buffer &buffer,
                                       size_t writtenSize);
};

} // namespace tt