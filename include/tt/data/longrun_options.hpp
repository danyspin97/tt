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

#ifndef TT_LONGRUN_OPTIONS_HPP_
#define TT_LONGRUN_OPTIONS_HPP_

#include <istream> // for ostream

#include "tt/data/service_options.hpp" // for ServiceOptions

namespace tt {

class LongrunOptions : public ServiceOptions {
public:
    [[nodiscard]] auto optional() const noexcept -> bool { return optional_; }
    void optional(bool optional) noexcept { optional_ = optional; }

    auto Dump(std::ostream &oss) const -> std::ostream & override;

private:
    bool optional_ = false;
};

} // namespace tt

#endif // TT_LONGRUN_OPTIONS_HPP_
