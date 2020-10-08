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

#include <string>
#include <vector>

#include "tt/data/service.hpp"
#include "tt/svc/types.hpp"

namespace tt {

class SpawnSupervise {
public:
    explicit SpawnSupervise(const Longrun &longrun,
                            std::vector<const char *> environment);

private:
    static auto GetExecArgs(const std::string &filename) -> std::vector<char *>;
    auto GetScriptFilename() -> std::string;
    void Spawn(const std::string &filename);
    void WriteScriptToFile(const std::string &filename);

    const Longrun &longrun_;
    std::vector<const char *> environment_;
};

} // namespace tt
