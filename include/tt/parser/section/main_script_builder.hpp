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

#include <string> // for string

#include "tt/data/main_script.hpp"              // for MainScript
#include "tt/parser/section/script_builder.hpp" // for ScriptBuilder

namespace tt {

class MainScriptBuilder : public ScriptBuilder {
public:
    using ScriptBuilder::ScriptBuilder;
    [[nodiscard]] auto main_script() const noexcept -> MainScript;
    [[nodiscard]] auto main_script() noexcept -> MainScript &&;

protected:
    [[nodiscard]] auto CreateScript() noexcept
        -> tl::expected<void, ParserError> override;
    [[nodiscard]] auto GetValidAttributes() const noexcept
        -> std::vector<std::string> override;
    [[nodiscard]] auto
    SetOptionalAttributeForMainScript(MainScript &main_script) const noexcept
        -> tl::expected<void, ParserError>;

private:
    std::optional<MainScript> main_script_;
};

} // namespace tt
