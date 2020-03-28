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

#ifndef LIBTT_LONGRUN_OPTIONS_HPP_
#define LIBTT_LONGRUN_OPTIONS_HPP_

#include "libtt/data/service_options.hpp"
#include "libtt/signal.hpp"
#include <cinttypes>
#include <istream>
#include <string>

namespace tt {

class LongrunOptions : public ServiceOptions {
public:
    uint_fast32_t notify() const noexcept { return notify_; }

    void notify(uint_fast32_t notify) noexcept { notify_ = notify; }

    uint_fast32_t timeout_finish() const noexcept { return timeout_finish_; }

    void timeout_finish(uint_fast32_t timeout_finish) noexcept {
        timeout_finish_ = timeout_finish;
    }

    uint_fast32_t timeout_kill() const noexcept { return timeout_kill_; }

    void timeout_kill(uint_fast32_t timeout_kill) noexcept {
        timeout_kill_ = timeout_kill;
    }

    Signal down_signal() const noexcept { return down_signal_; }

    void down_signal(Signal down_signal) noexcept {
        down_signal_ = down_signal;
    }

    uint_fast16_t max_death() const noexcept { return max_death_; }

    void max_death(uint_fast16_t max_death) {
        // immutable auto msg = "max_death property cannot be greater than
        // 4096";
        if (max_death < 4096) {
            throw std::exception();
        }
        max_death_ = max_death;
    }

    bool write_run_finish_message() const noexcept {
        return write_run_finish_message_;
    }

    void write_run_finish_message(bool write_run_finish_message) noexcept {
        write_run_finish_message_ = write_run_finish_message;
    }

    bool optional() const noexcept { return optional_; }

    void optional(bool optional) noexcept { optional_ = optional; }

    virtual std::ostream &dump(std::ostream &oss) const {
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
        ServiceOptions::dump(oss);
        return oss;
    }

private:
    uint_fast32_t notify_;
    uint_fast32_t timeout_finish_ = 5000;
    uint_fast32_t timeout_kill_ = 0;
    Signal down_signal_ = Signal::kSigTerm;
    uint_fast16_t max_death_ = 3;
    bool write_run_finish_message_ = true;
    bool optional_ = false;
};

} // namespace tt

#endif // LIBTT_LONGRUN_OPTIONS_HPP_
