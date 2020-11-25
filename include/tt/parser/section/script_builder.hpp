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
    explicit ScriptBuilder(const std::string &section);
    auto script() -> Script;
    auto EndParsing() -> tl::expected<void, ParserError> override;

protected:
    virtual auto CreateScript() -> tl::expected<void, ParserError>;
    virtual auto GetScriptAttributes()
        -> tl::expected<std::pair<Script::Type, std::string>, ParserError>;

    auto GetValidAttributes() const -> std::vector<std::string> override;
    auto GetValidArrayAttributes() const -> std::vector<std::string> override {
        return {};
    }
    auto GetValidCodeAttributes() const -> std::vector<std::string> override;
    auto GetParsedType() const -> tl::expected<Script::Type, ParserError>;
    auto SetOptionalAttributeForScript(Script &script) const
        -> tl::expected<void, ParserError>;

private:
    std::optional<Script> script_;
};

} // namespace tt

#endif // TT_SCRIPT_BUILDER_HPP_
