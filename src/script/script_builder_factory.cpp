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

#include "tt/script/script_builder_factory.hpp"

#include <cassert> // for assert

#include "tt/script/bash_script_builder.hpp"  // for BashScriptBuilder
#include "tt/script/path_script_builder.hpp"  // for PathScriptBuilder
#include "tt/script/sh_script_builder.hpp"    // for ShScriptBuilder
#include "tt/script/shell_script_builder.hpp" // for ShellScriptBuilder

auto tt::ScriptBuilderFactory::GetScriptBuilder(Script::Type type)
    -> std::unique_ptr<ShellScriptBuilder> {
    switch (type) {
    case Script::Type::Bash:
        return std::make_unique<BashScriptBuilder>();
    case Script::Type::Execline:
        // TODO
    case Script::Type::Path:
        return std::make_unique<PathScriptBuilder>();
    case Script::Type::SH:
        return std::make_unique<ShScriptBuilder>();
    default:
        assert(false);
    }
}
