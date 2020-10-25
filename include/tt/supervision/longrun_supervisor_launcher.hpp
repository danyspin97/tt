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
#include "tt/supervision/types.hpp"

namespace tt {

class Dirs;

class LongrunSupervisorLauncher {
public:
    LongrunSupervisorLauncher(const Longrun &longrun,
                              const std::shared_ptr<Dirs> &dirs);
    void Launch();

private:
    Longrun longrun_;
    std::vector<const char *> environment_;
    std::filesystem::path filename_;
};

} // namespace tt
