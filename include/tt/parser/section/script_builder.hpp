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

#ifndef TT_SCRIPT_BUILDER_HPP_
#define TT_SCRIPT_BUILDER_HPP_

#include <string>

#include "tt/data/script.hpp"
#include "tt/parser/section/code_section_builder.hpp"

namespace tt {

class ScriptBuilder : public CodeSectionBuilder {
public:
    explicit ScriptBuilder(const std::string &section) noexcept;
    [[nodiscard]] auto script() const noexcept -> Script;
    [[nodiscard]] auto EndParsing() noexcept
        -> tl::expected<void, ParserError> override;

protected:
    virtual auto CreateScript() -> tl::expected<void, ParserError>;
    [[nodiscard]] auto GetScriptAttributes() noexcept
        -> tl::expected<std::pair<Script::Type, std::string>, ParserError>;

    [[nodiscard]] auto GetValidAttributes() const noexcept
        -> std::vector<std::string> override;
    [[nodiscard]] auto GetValidArrayAttributes() const noexcept
        -> std::vector<std::string> override {
        return {};
    }
    [[nodiscard]] auto GetValidCodeAttributes() const noexcept
        -> std::vector<std::string> override;
    [[nodiscard]] auto GetParsedType() const noexcept
        -> tl::expected<Script::Type, ParserError>;
    [[nodiscard]] auto
    SetOptionalAttributeForScript(Script &script) const noexcept
        -> tl::expected<void, ParserError>;

private:
    std::optional<Script> script_;
};

} // namespace tt

#endif // TT_SCRIPT_BUILDER_HPP_
