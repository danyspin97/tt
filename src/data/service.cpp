/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include "tt/data/service.hpp"

auto operator<<(std::ostream &oss, const tt::Service &service)
    -> std::ostream & {
    std::visit(
        [&oss](auto &arg) {
            if constexpr (not std::is_same_v<std::decay_t<decltype(arg)>,
                                             std::monostate>) {
                arg.Dump(oss);
            }
        },
        service);

    return oss;
}
