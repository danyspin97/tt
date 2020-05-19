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

#ifndef TT_LONGRUN_HPP_
#define TT_LONGRUN_HPP_

#include "tt/data/logger_script.hpp"
#include "tt/data/longrun_options.hpp"
#include "tt/data/script.hpp"
#include "tt/data/service_impl.hpp"
#include <optional>
#include <string>

namespace tt {

class Longrun : public ServiceImpl {
public:
    [[nodiscard]] Script run() const noexcept { return run_; }

    [[nodiscard]] std::optional<Script> finish() const noexcept {
        return finish_;
    }

    void finish(const Script &finish) noexcept { finish_.emplace(finish); }

    [[nodiscard]] std::optional<LoggerScript> logger() const noexcept {
        return logger_;
    }

    void logger(const LoggerScript &logger) noexcept {
        logger_.emplace(logger);
    }

    Longrun(const std::string &name, const std::string &polish_name,
            const std::string &description, const std::string &path,
            LongrunOptions options, const Script &run);

    std::ostream &Dump(std::ostream &oss) const override;

private:
    Script run_;
    std::optional<Script> finish_;
    std::optional<LoggerScript> logger_;
};

} // namespace tt

std::ostream &operator<<(std::ostream &oss, const tt::Longrun &longrun);

#endif // TT_LONGRUN_HPP_
