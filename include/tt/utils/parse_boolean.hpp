
#pragma once

#include <string>

#include "tt/exception.hpp"

namespace tt::utils {

class BooleanParseException : public Exception {
public:
    explicit BooleanParseException(const std::string &msg) : Exception(msg) {}
};

auto ParseBoolean(const std::string &value) -> bool;

} // namespace tt::utils
