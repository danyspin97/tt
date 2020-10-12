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

#include <array>
#include <filesystem>
#include <string>

#include "tt/data/environment.hpp"

namespace tt {

class ShellScriptBuilder {
public:
    ShellScriptBuilder() = default;
    virtual ~ShellScriptBuilder() = default;

    [[nodiscard]] auto environment() const -> const Environment &;

    // Returns the file to run (i.e. /bin/sh) and its arguments
    // (i.e. "sh", "-c", "execute...")
    [[nodiscard]] auto script(const std::string &execute,
                              const Environment &environment)
        -> std::pair<std::string, std::vector<std::string>>;

protected:
    // No newline are applied
    void AppendCode(const std::string &code);
    void PrependCode(const std::string &code);

    // Use AppendCode() and PrependCode() to add code to the execute script
    virtual void ApplyModifiers() = 0;
    // Return the shell to execute, like /bin/bash
    // Must be absolute
    [[nodiscard]] virtual std::filesystem::path GetFileToExecute() = 0;

private:
    std::string script_;
    Environment env_;
};

} // namespace tt
