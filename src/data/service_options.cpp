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

#include "tt/data/service_options.hpp"

#include <algorithm>
#include <iterator>

using std::copy;
using std::ostream;
using std::ostream_iterator;
using std::string;

using tt::ServiceOptions;

ostream &ServiceOptions::Dump(ostream &oss) const {
    if (depends_.size() != 0) {
        oss << "depends = ( ";
        copy(depends_.begin(), depends_.end(), ostream_iterator<string>(oss));
        return oss << " )";
    }

    return oss;
}

ostream &operator<<(ostream &oss, const ServiceOptions &options) {
    return options.Dump(oss);
}
