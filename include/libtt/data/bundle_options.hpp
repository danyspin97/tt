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

#ifndef LIBTT_BUNDLE_OPTIONS_HPP_
#define LIBTT_BUNDLE_OPTIONS_HPP_

#include <vector>
#include <string>

#include "libtt/data/service_options.hpp"

namespace tt {

class BundleOptions : public ServiceOptions
{
public:
    std::vector<std::string> contents()
    {
        return contents_;
    }

    void contents(std::vector<std::string> contents)
    {
        contents_ = contents;
    }

    std::ostream & dump(std::ostream &oss) const {
        oss<<"contents = ( ";
        std::copy(contents_.begin(), contents_.end(), std::ostream_iterator<std::string>(oss, " "));
        return oss << " )";
    }

private:
    std::vector<std::string> contents_;
};

} // namespace tt

#endif // LIBTT_BUNDLE_OPTIONS_HPP_
