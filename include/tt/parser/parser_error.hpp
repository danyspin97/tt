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
