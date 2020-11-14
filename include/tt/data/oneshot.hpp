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

#ifndef TT_ONESHOT_HPP_
#define TT_ONESHOT_HPP_

#include <iosfwd>   // for ostream
#include <optional> // for optional
#include <string>   // for string

#include "bitsery/ext/growable.h"     // for Growable
#include "bitsery/ext/inheritance.h"  // for BaseClass
#include "bitsery/ext/std_optional.h" // for StdOptional

#include "tt/data/environment.hpp"     // for Environment
#include "tt/data/main_script.hpp"     // for MainScript
#include "tt/data/oneshot_options.hpp" // for OneshotOptions
#include "tt/data/script.hpp"          // for Script
#include "tt/data/service_impl.hpp"    // for ServiceImpl

namespace bitsery {
class Access;
} // namespace bitsery

namespace tt {

class Oneshot : public ServiceImpl {
public:
    Oneshot(std::string &&name, std::string &&description, std::string &&path,
            OneshotOptions &&options, Environment &&environment,
            MainScript &&start);

    auto operator==(const Oneshot &rhs) const -> bool;
    auto operator!=(const Oneshot &rhs) const { return !operator==(rhs); }

    [[nodiscard]] auto environment() const noexcept -> Environment {
        return environment_;
    }

    [[nodiscard]] auto start() const noexcept -> MainScript { return start_; }

    [[nodiscard]] auto stop() const noexcept -> std::optional<Script> {
        return stop_;
    }

    void stop(const Script &stop) { stop_.emplace(stop); }

    [[nodiscard]] auto options() const noexcept -> OneshotOptions {
        return options_;
    }

    auto Dump(std::ostream &oss) const -> std::ostream & override;

private:
    friend class bitsery::Access;
    Oneshot() = default;

    template <typename S> void serialize(S &serializer) {
        serializer.ext(*this, bitsery::ext::BaseClass<ServiceImpl>{});
        serializer.ext(environment_, bitsery::ext::Growable{},
                       [](S &serializer, Environment &environment) {
                           serializer.object(environment);
                       });
        serializer.ext(start_, bitsery::ext::Growable{},
                       [](S &serializer, MainScript &main_script) {
                           serializer.object(main_script);
                       });
        serializer.ext(stop_, bitsery::ext::StdOptional{},
                       [](S &serializer, Script &script) {
                           serializer.ext(script, bitsery::ext::Growable{},
                                          [](S &serializer, Script &script) {
                                              serializer.object(script);
                                          });
                       });
        serializer.object(options_);
    }

    OneshotOptions options_;
    Environment environment_;
    MainScript start_;
    std::optional<Script> stop_;
};

} // namespace tt

auto operator<<(std::ostream &oss, const tt::Oneshot &oneshot)
    -> std::ostream &;

#endif // TT_ONESHOT_HPP_
