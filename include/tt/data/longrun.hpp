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

#ifndef TT_LONGRUN_HPP_
#define TT_LONGRUN_HPP_

#include <filesystem> // for path
#include <iosfwd>     // for ostream
#include <optional>   // for optional
#include <string>     // for string

#include "bitsery/ext/growable.h"     // for Growable
#include "bitsery/ext/inheritance.h"  // for BaseClass
#include "bitsery/ext/std_optional.h" // for StdOptional

#include "tt/data/environment.hpp"       // for Environment
#include "tt/data/long_lived_script.hpp" // for LongLivedScript
#include "tt/data/longrun_options.hpp"   // for LongrunOptions
#include "tt/data/script.hpp"            // for Script
#include "tt/data/service_impl.hpp"      // for ServiceImpl

namespace bitsery {
class Access;
} // namespace bitsery

namespace tt {

namespace utils {
template <typename T> auto Deserialize(const std::filesystem::path &) -> T;
}

class Longrun : public ServiceImpl {
public:
    Longrun(std::string &&name, std::string &&description, std::string &&path,
            LongrunOptions &&options, Environment &&environment,
            LongLivedScript &&run);

    auto operator==(const Longrun &rhs) const -> bool;
    auto operator!=(const Longrun &rhs) const { return !operator==(rhs); }

    [[nodiscard]] auto environment() const noexcept -> Environment {
        return environment_;
    }
    [[nodiscard]] auto run() const noexcept -> LongLivedScript { return run_; }

    [[nodiscard]] auto finish() const noexcept -> std::optional<Script> {
        return finish_;
    }

    void finish(const Script &finish) noexcept { finish_.emplace(finish); }

    [[nodiscard]] auto options() const noexcept -> LongrunOptions {
        return options_;
    }

    auto Dump(std::ostream &oss) const -> std::ostream & override;

private:
    friend class bitsery::Access;
    friend auto utils::Deserialize<Longrun>(const std::filesystem::path &)
        -> Longrun;
    Longrun() = default;

    template <typename S> void serialize(S &serializer) {
        serializer.ext(*this, bitsery::ext::BaseClass<ServiceImpl>{});
        serializer.ext(environment_, bitsery::ext::Growable{},
                       [](S &serializer, Environment &environment) {
                           serializer.object(environment);
                       });
        serializer.ext(run_, bitsery::ext::Growable{},
                       [](S &serializer, LongLivedScript &long_lived_script) {
                           serializer.object(long_lived_script);
                       });
        serializer.ext(finish_, bitsery::ext::StdOptional{},
                       [](S &serializer, Script &script) {
                           serializer.ext(script, bitsery::ext::Growable{},
                                          [](S &serializer, Script &script) {
                                              serializer.object(script);
                                          });
                       });
        serializer.object(options_);
    }

    LongrunOptions options_;
    Environment environment_;
    LongLivedScript run_;
    std::optional<Script> finish_;
};

} // namespace tt

auto operator<<(std::ostream &oss, const tt::Longrun &longrun)
    -> std::ostream &;

#endif // TT_LONGRUN_HPP_
