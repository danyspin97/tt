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

#include "tt/data/oneshot_options.hpp"

#include <ostream> // for operator<<, ostream, basic_ostream

auto tt::OneshotOptions::operator==(const OneshotOptions &rhs) const -> bool {
    return optional_ == rhs.optional_;
}

auto tt::OneshotOptions::Dump(std::ostream &oss) const -> std::ostream & {
    ServiceOptions::Dump(oss) << "\n";
    return oss << "\noptional = " << optional();
}
