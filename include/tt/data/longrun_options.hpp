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

#ifndef TT_LONGRUN_OPTIONS_HPP_
#define TT_LONGRUN_OPTIONS_HPP_

#include "tt/data/service_options.hpp"
#include "tt/signal.hpp"
#include <cinttypes>
#include <istream>
#include <string>

namespace tt {

class LongrunOptions : public ServiceOptions {
public:
    [[nodiscard]] auto notify() const noexcept -> uint_fast32_t { return notify_; }

    void notify(uint_fast32_t notify) noexcept { notify_ = notify; }

    [[nodiscard]] auto timeout_finish() const noexcept -> uint_fast32_t {
        return timeout_finish_;
    }

    void timeout_finish(uint_fast32_t timeout_finish) noexcept {
        timeout_finish_ = timeout_finish;
    }

    [[nodiscard]] auto timeout_kill() const noexcept -> uint_fast32_t {
        return timeout_kill_;
    }

    void timeout_kill(uint_fast32_t timeout_kill) noexcept {
        timeout_kill_ = timeout_kill;
    }

    [[nodiscard]] auto down_signal() const noexcept -> Signal { return down_signal_; }

    void down_signal(Signal down_signal) noexcept {
        down_signal_ = down_signal;
    }

    [[nodiscard]] auto max_death() const noexcept -> uint_fast16_t {
        return max_death_;
    }

    void max_death(uint_fast16_t max_death);

    [[nodiscard]] auto write_run_finish_message() const noexcept -> bool {
        return write_run_finish_message_;
    }

    void write_run_finish_message(bool write_run_finish_message) noexcept {
        write_run_finish_message_ = write_run_finish_message;
    }

    [[nodiscard]] auto optional() const noexcept -> bool { return optional_; }

    void optional(bool optional) noexcept { optional_ = optional; }

    auto Dump(std::ostream &oss) const -> std::ostream & override;

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

#endif // TT_LONGRUN_OPTIONS_HPP_
