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

#include <filesystem> // for path
#include <string>     // for string
#include <vector>     // for vector

#include "tt/script/shell_script_builder.hpp" // for ShellScriptBuilder

namespace tt {
class Environment;

class PathScriptBuilder : public ShellScriptBuilder {
public:
    using ShellScriptBuilder::ShellScriptBuilder;

    [[nodiscard]] auto script(const std::string &execute,
                              const Environment &environment)
        -> std::vector<std::string> override;

protected:
    void ApplyModifiers() override;
    [[nodiscard]] static auto GetAbsolutePath(const std::string &path)
        -> std::filesystem::path;
    [[nodiscard]] auto GetFileToExecute() -> std::filesystem::path override;
};

} // namespace tt
