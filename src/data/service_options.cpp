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

#include "tt/data/service_options.hpp"

#include <algorithm>
#include <iterator>

#include "tt/parser/define.hpp"

using std::copy;
using std::ostream;
using std::ostream_iterator;
using std::string;

using tt::ServiceOptions;

auto ServiceOptions::Dump(ostream &oss) const -> ostream & {
    if (!depends_.empty()) {
        oss << "depends = " << tt::kArrayOpenToken << " ";
        copy(depends_.begin(), depends_.end(),
             ostream_iterator<string>(oss, " "));
        return oss << tt::kArrayCloseToken;
    }

    return oss;
}

auto operator<<(ostream &oss, ServiceOptions const &options) -> ostream & {
    return options.Dump(oss);
}
