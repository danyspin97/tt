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

#include "tt/path/user_dirs.hpp"

#include <algorithm> // for max
#include <optional>  // for optional

#include "xdg.hpp" // for BaseDirectories

#include "tt/path/directory_names.hpp" // for kServiceDirectoryName, kSuper...

tt::UserDirs::UserDirs() {
    xdg::BaseDirectories xdg_dirs{};
    std::optional<std::filesystem::path> runtime_dir = xdg_dirs.Runtime();
    if (runtime_dir) {
        livedir_ = runtime_dir.value() / "tt";
    } else {
        livedir_ = xdg_dirs.CacheHome() / "tt";
    }

    confdir_ = xdg_dirs.ConfigHome() / "tt";
    statedir_ = xdg_dirs.DataHome() / "tt" / "state";
    logdir_ = xdg_dirs.DataHome() / "tt" / "log";

    if (!std::filesystem::exists(livedir_)) {
        std::filesystem::create_directories(livedir_);
    }
    if (!std::filesystem::exists(confdir_)) {
        std::filesystem::create_directories(confdir_);
    }
    if (!std::filesystem::exists(statedir_)) {
        std::filesystem::create_directories(statedir_);
    }
    if (!std::filesystem::exists(logdir_)) {
        std::filesystem::create_directories(logdir_);
    }
}

auto tt::UserDirs::confdir() const -> std::filesystem::path { return confdir_; }

auto tt::UserDirs::livedir() const -> std::filesystem::path { return livedir_; }

auto tt::UserDirs::logdir() const -> std::filesystem::path { return logdir_; }

auto tt::UserDirs::servicedirs() const -> std::vector<std::filesystem::path> {
    std::vector<std::filesystem::path> dirs{confdir() / kServiceDirectoryName};
    auto system_dirs = SystemDirs::servicedirs();
    dirs.insert(dirs.end(), system_dirs.begin(), system_dirs.end());
    return dirs;
}

auto tt::UserDirs::statedir() const -> std::filesystem::path {
    return statedir_;
}

auto tt::UserDirs::supervisedir() const -> std::filesystem::path {
    auto dir = livedir() / kSuperviseDirectoryName;
    std::filesystem::create_directories(dir);
    return dir;
}

auto tt::UserDirs::services_suffix() const -> std::string { return ".user"; }
