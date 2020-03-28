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

#ifndef LIBTT_SERVICE_OPTIONS_HPP_
#define LIBTT_SERVICE_OPTIONS_HPP_

#include <istream>
#include <string>
#include <vector>

namespace tt {

class ServiceOptions
{
public:
    std::vector<std::string> dependencies()
    {
        return depends_;
    }

    void dependencies(std::vector<std::string> depends)
    {
        depends_ = depends;
    }

    virtual ~ServiceOptions() {}

    virtual std::ostream &dump(std::ostream &oss) const {

        if (depends_.size() != 0)
        {
            return oss << "depends = ( " << std::string{depends_.begin(), depends_.end()} << " )";
        }

        return oss;
    }

private:
        std::vector<std::string> depends_;
};

std::ostream &operator<<(std::ostream &oss, const ServiceOptions &options) { return options.dump(oss); }

} // namespace tt

#endif // LIBTT_SERVICE_OPTIONS_HPP_
