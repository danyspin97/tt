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

#include "tt/script/shell_script_builder.hpp"

#include "fmt/format.h"

auto tt::ShellScriptBuilder::environment() const -> const Environment & {
    return env_;
}

auto tt::ShellScriptBuilder::script(const std::string &execute,
                                    const Environment &environment)
    -> std::pair<const char *, std::array<const char *, 3>> {
    script_ = execute;
    env_ = environment;
    ApplyModifiers();

    // This leaks, but it will be passed to execvp
    auto *bin = new std::filesystem::path{GetFileToExecute()};
    assert(bin->is_absolute());
    auto shell = new std::filesystem::path{bin->filename()};
    return std::make_pair(
        bin->c_str(),
        std::array<const char *, 3>{shell->c_str(), "-c",
                                    (new std::string{script_})->c_str()});
}

void tt::ShellScriptBuilder::AppendCode(const std::string &code) {
    fmt::format_to(std::back_inserter(script_), "{}", code);
}

void tt::ShellScriptBuilder::PrependCode(const std::string &code) {
    std::string new_script;
    fmt::format_to(std::back_inserter(new_script), "{}{}", code, script_);
    script_ = std::move(new_script);
}
