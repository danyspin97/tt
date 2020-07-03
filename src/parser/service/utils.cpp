
#include "tt/parser/service/utils.hpp"

#include <fstream>

auto tt::GetLinesFromFile(const std::string &path) -> std::vector<std::string> {
    std::ifstream service_file(path);

    std::vector<std::string> lines;
    std::string line;
    while (getline(service_file, line)) {
        lines.push_back(line);
    }

    return lines;
}
