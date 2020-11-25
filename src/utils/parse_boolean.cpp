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

#include "tt/utils/parse_boolean.hpp"

#include "fmt/format.h" // for format

#include "tl/expected.hpp" // for expected

auto tt::utils::ParseBoolean(const std::string &value)
    -> tl::expected<bool, std::string> {
    if (value == "yes") {
        return true;
    }
    if (value == "no") {
        return false;
    }

    return tl::make_unexpected(
        fmt::format("A boolean value shall be either 'yes' or 'no' the value "
                    "given was '{}'",
                    value));
}
