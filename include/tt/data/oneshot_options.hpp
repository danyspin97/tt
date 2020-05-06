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

#ifndef TT_ONESHOT_OPTIONS_HPP_
#define TT_ONESHOT_OPTIONS_HPP_

#include <string>

#include "tt/data/service_options.hpp"

namespace tt {

class OneshotOptions : public ServiceOptions {
public:
    [[nodiscard]] bool optional() const noexcept { return optional_; }

    void optional(bool optional) { optional_ = optional; }

    [[nodiscard]] bool write_start_stop_messages() const noexcept {
        return write_start_stop_messages_;
    }

    void write_start_stop_messages(bool new_value) {
        write_start_stop_messages_ = new_value;
    }

    std::ostream &Dump(std::ostream &oss) const override;

private:
    bool optional_ = false;
    bool write_start_stop_messages_ = true;
};

} // namespace tt

#endif // TT_BUNDLE_OPTIONS_HPP_
