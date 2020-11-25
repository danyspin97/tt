
#pragma once

#include <string> // for std::string

#include "tl/expected.hpp" // for expected

namespace tt {

class ParserError {
public:
    enum class Type {
        ArrayValueNotClosed,
        ArrayValueIsEmpty,
        ArrayValueAfterClosingToken,
        BundleEmptyContents,
        CodeSectionInvalidLine,
        CodeSectionInvalidKey,
        EmptyKey,
        EnvironmentKeyAlreadySet,
        EnvironmentKeyNotValid,
        FoundLineBeforeMainSection,
        InvalidBoolean,
        InvalidInteger,
        InvalidKeyInSection,
        InvalidLine,
        InvalidScriptType,
        InvalidServiceType,
        InvalidSignal,
        InvalidSection,
        KeyValueParserLineInvalid,
        MainSectionMissing,
        MissingExecute,
        MissingMainSection,
        MissingScriptType,
        MissingServiceType,
        MissingStartSection,
        MissingRunSection,
        MissingValueInMainSection,
        SectionKeyAlreadySet,
        ServiceNotFound,
    };

    ParserError(Type type, std::string err_msg) noexcept;
    ParserError(ParserError &&rhs) noexcept = default;
    ParserError(const ParserError &rhs) noexcept = default;
    [[nodiscard]] auto msg() const noexcept -> std::string { return err_msg_; }
    [[nodiscard]] auto type() const noexcept -> Type { return type_; }
    void Chain(const std::string &err_msg) noexcept;

private:
    Type type_;
    std::string err_msg_;
};

template <typename T>
[[nodiscard]] inline auto make_parser_error(ParserError::Type type,
                                            std::string err_msg) noexcept
    -> tl::expected<T, ParserError> {
    return tl::make_unexpected(ParserError(type, std::move(err_msg)));
}

template <typename T>
[[nodiscard]] inline auto
chain_parser_error(ParserError error, const std::string &err_msg) noexcept
    -> tl::expected<T, ParserError> {
    error.Chain(err_msg);
    return tl::make_unexpected(std::move(error));
}

} // namespace tt
