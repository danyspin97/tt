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

#ifndef TT_ONESHOT_HPP_
#define TT_ONESHOT_HPP_

#include <optional>

#include "tt/data/oneshot_options.hpp"
#include "tt/data/script.hpp"
#include "tt/data/service_impl.hpp"

namespace tt {

class Oneshot : public ServiceImpl {
public:
    [[nodiscard]] Script start() const noexcept { return start_; }
    Oneshot(const std::string &name, const std::string &polish_name,
            const std::string &description, const std::string &path,
            OneshotOptions options, Environment environment,
            const Script &start);
    Oneshot(Oneshot &) = default;
    Oneshot(Oneshot &&) noexcept = default;
    Oneshot(Oneshot const &) noexcept = default;
    Oneshot &operator=(Oneshot &&) = default;
    Oneshot &operator=(Oneshot const &) = default;

    [[nodiscard]] Environment environment() const noexcept {
        return environment_;
    }

    [[nodiscard]] std::optional<Script> stop() const noexcept { return stop_; }

    void stop(const Script &stop) { stop_.emplace(stop); }

    std::ostream &Dump(std::ostream &oss) const override;

private:
    Environment environment_;
    Script start_;
    std::optional<Script> stop_;
};

} // namespace tt

std::ostream &operator<<(std::ostream &oss, const tt::Oneshot &oneshot);

#endif // TT_ONESHOT_HPP_
