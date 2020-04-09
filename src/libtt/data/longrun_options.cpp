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

#include "libtt/data/longrun_options.hpp"

#include "libtt/data/exception.hpp"

using std::ostream;

using tt::LongrunOptions;

void LongrunOptions::max_death(uint_fast16_t max_death) {
    if (max_death > 4096) {
        throw MaxDeathTooBigException();
    }
    max_death_ = max_death;
}

ostream &LongrunOptions::Dump(ostream &oss) const {
    oss << "\ntimeout_finish = " << timeout_finish();
    oss << "\ntimeout_kill = " << timeout_kill();
    // TODO: Convert a tt::Signal to string
    // o << "\ndown_signal = " << down_signal();
    oss << "\nmax_death = " << max_death();
    oss << "\nwrite_message = " << write_run_finish_message();
    oss << "\noptional = " << optional();
    if (notify() != 0) {
        oss << "\nnotify = " << notify();
    }
    ServiceOptions::Dump(oss);
    return oss;
}
