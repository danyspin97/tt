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

#include "tt/script/path_script_builder.hpp"

#include <algorithm> // for max
#include <cassert>   // for assert
#include <cstdlib>   // for getenv
#include <iterator>  // for back_inserter

#include "fmt/format.h" // for format_to

#include "tt/utils/split_path.hpp" // for SplitPath
#include "tt/utils/trim.hpp"       // for trim_copy

namespace tt {
class Environment;
} // namespace tt

// This function is never called
void tt::PathScriptBuilder::ApplyModifiers() {}

[[nodiscard]] auto tt::PathScriptBuilder::script(const std::string &execute,
                                                 const tt::Environment &
                                                 /*environment*/)
    -> std::vector<std::string> {
    auto script = utils::trim_copy(execute);
    auto i = script.find(' ');
    std::string path{script.substr(0, i)};
    std::vector<std::string> argv;
    argv.push_back(GetAbsolutePath(path));
    if (i == std::string::npos) {
        return argv;
    }
    bool backslash = false;
    char quote_char = 0;
    std::string res{};
    for (char c : script.substr(i)) {
        if (!backslash && c == '\\') {
            backslash = true;
            continue;
        }
        if (!backslash && (c == '"' || c == '\'') && quote_char == 0) {
            quote_char = c;
            continue;
        }
        if (!backslash && c == quote_char) {
            quote_char = 0;
            continue;
        }
        // From here we don't care about the backslash
        if (backslash) {
            // But we also know that this char isn't a backslash
            // reset the status and
            fmt::format_to(std::back_inserter(res), "{}", '\\');
            backslash = false;
        }
        // Spaces are part of the string when inside the quotes
        if (c != ' ' || quote_char != 0) {
            fmt::format_to(std::back_inserter(res), "{}", c);
            continue;
        }

        if (!res.empty()) {
            argv.emplace_back(std::string{res});
            res.clear();
        }
    }
    argv.emplace_back(std::string{res});

    return argv;
}

auto tt::PathScriptBuilder::GetAbsolutePath(const std::string &path)
    -> std::filesystem::path {
    std::filesystem::path filepath{path};

    if (filepath.is_absolute()) {
        return filepath;
    }

    // TODO: Use tt.env or Status::env()
    const char *env_path = getenv("PATH");
    std::string path_to_search;
    if (env_path != nullptr) {
        path_to_search = std::string{env_path};
    } else {
        path_to_search = "/bin";
    }
    auto paths = utils::SplitPath(path_to_search);
    for (const auto &p : paths) {
        std::filesystem::path new_path{p / filepath};
        if (std::filesystem::exists(new_path)) {
            // TODO: Check file is executable
            return new_path;
        }
    }

    // This will fail and the user will get notified
    // TODO: Generate an exception
    return filepath;
}

auto tt::PathScriptBuilder::GetFileToExecute() -> std::filesystem::path {
    assert(false);
    return {};
}
