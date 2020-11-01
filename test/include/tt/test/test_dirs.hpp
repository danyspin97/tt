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

#include "tt/path/system_dirs.hpp"

namespace tt::test {

class TestDirs : public SystemDirs {
public:
    using SystemDirs::SystemDirs;

    [[nodiscard]] auto confdir() const -> std::filesystem::path override {
        return "tmp/conf";
    }
    [[nodiscard]] auto livedir() const -> std::filesystem::path override {
        return "tmp/live";
    }
    [[nodiscard]] auto logdir() const -> std::filesystem::path override {
        return "tmp/log";
    }
    [[nodiscard]] auto servicedirs() const
        -> std::vector<std::filesystem::path> override {
        return {"tmp/../data/"};
    }
    [[nodiscard]] auto statedir() const -> std::filesystem::path override {
        return "tmp/state";
    }
    [[nodiscard]] auto supervisedir() const -> std::filesystem::path override {
        return "tmp/supervise";
    }
    [[nodiscard]] auto services_suffix() const -> std::string override {
        return "tmp/.test";
    }
};

} // namespace tt::test
