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

#include <optional>
#include <string>

#include "bitsery/ext/inheritance.h"
#include "bitsery/ext/std_optional.h"

#include "tt/data/logger_script.hpp"
#include "tt/data/longrun_options.hpp"
#include "tt/data/script.hpp"
#include "tt/data/service_impl.hpp"

namespace tt {

class Longrun : public ServiceImpl {
public:
    Longrun(const std::string &name, const std::string &description,
            const std::string &path, LongrunOptions options,
            Environment environment, const Script &run);

    [[nodiscard]] Environment environment() const noexcept {
        return environment_;
    }
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

    std::ostream &Dump(std::ostream &oss) const override;

private:
    friend class bitsery::Access;
    Longrun() = default;

    template <typename S> void serialize(S &serializer) {
        serializer.ext(*this, bitsery::ext::BaseClass<ServiceImpl>{});
        serializer.object(environment_);
        serializer.object(run_);
        serializer.ext(
            finish_, bitsery::ext::StdOptional{},
            [](S &serializer, Script &script) { serializer.object(script); });
        serializer.ext(logger_, bitsery::ext::StdOptional{},
                       [](S &serializer, LoggerScript &logger_script) {
                           serializer.object(logger_script);
                       });
    }

    Environment environment_;
    Script run_;
    std::optional<Script> finish_;
    std::optional<LoggerScript> logger_;
};

} // namespace tt

std::ostream &operator<<(std::ostream &oss, const tt::Longrun &longrun);

#endif // TT_LONGRUN_HPP_
