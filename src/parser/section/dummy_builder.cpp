
#include "tt/parser/section/dummy_builder.hpp"

#include "tt/parser/parser_error.hpp"  // for ParserError
#include "tt/parser/section/utils.hpp" // for IsEmptyLine

tt::DummyBuilder::DummyBuilder() noexcept : SectionBuilder("dummy") {}

auto tt::DummyBuilder::ParseLine(const std::string &line) noexcept
    -> tl::expected<void, ParserError> {
    if (IsEmptyLine(line)) {
        return {};
    }

    return make_parser_error<void>(
        ParserError::Type::FoundLineBeforeMainSection,
        "Found line before [main] section");
}
