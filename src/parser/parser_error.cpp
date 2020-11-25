
#include "tt/parser/parser_error.hpp"

#include "fmt/format.h"

tt::ParserError::ParserError(Type type, std::string err_msg) noexcept
    : type_(type), err_msg_(std::move(err_msg)) {}

void tt::ParserError::Chain(const std::string &err_msg) noexcept {
    fmt::format_to(std::back_inserter(err_msg_), err_msg);
}
