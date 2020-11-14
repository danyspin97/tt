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

#include "tt/data/bundle_options.hpp"

#include <algorithm> // for copy
#include <iterator>  // for ostream_iterator
#include <ostream>   // for operator<<, ostream, basic_ostream

using std::copy;
using std::ostream;
using std::ostream_iterator;
using std::string;

using tt::BundleOptions;

auto BundleOptions::operator==(const BundleOptions &rhs) const -> bool {
    return contents_ == rhs.contents_;
}

auto BundleOptions::Dump(ostream &oss) const -> ostream & {
    oss << "contents = ( ";
    copy(contents_.begin(), contents_.end(),
         ostream_iterator<string>(oss, " "));
    return oss << " )";
}
