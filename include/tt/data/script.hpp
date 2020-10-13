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

#ifndef TT_DATA_SCRIPT_HPP_
#define TT_DATA_SCRIPT_HPP_

#include <cassert>
#include <optional>
#include <string>

#include "bitsery/ext/std_optional.h"
#include "bitsery/traits/string.h"

#include "tt/data/environment.hpp"
#include "tt/signal.hpp"

namespace tt {

class Script {
public:
    enum class Type : uint16_t {
        Bash,
        Execline,
        Path,
    };

    [[nodiscard]] auto type() const noexcept -> Type { return type_; }
    [[nodiscard]] auto execute() const noexcept -> std::string {
        return execute_;
    }
    [[nodiscard]] auto user() const noexcept -> std::optional<std::string> {
        return user_;
    }
    void user(const std::string &user) noexcept { user_ = user; }
    [[nodiscard]] auto group() const noexcept -> std::optional<std::string> {
        return group_;
    }
    void group(const std::string &group) noexcept { group_ = group; }
    Script(Type type, std::string &&execute);
    Script(Type type, std::string &&execute, std::string &&user,
           std::string &&group);
    virtual ~Script() = default;
    virtual auto Dump(std::ostream &oss) const -> std::ostream &;

    [[nodiscard]] auto timeout() const noexcept -> uint_fast32_t {
        return timeout_;
    }
    void timeout(uint_fast32_t timeout) noexcept { timeout_ = timeout; }

    [[nodiscard]] auto timeout_kill() const noexcept -> uint_fast32_t {
        return timeout_kill_;
    }
    void timeout_kill(uint_fast32_t timeout_kill) noexcept {
        timeout_kill_ = timeout_kill;
    }

    [[nodiscard]] auto down_signal() const noexcept -> Signal {
        return down_signal_;
    }
    void down_signal(Signal down_signal) noexcept {
        down_signal_ = down_signal;
    }

    [[nodiscard]] auto max_death() const noexcept -> uint_fast16_t {
        return max_death_;
    }
    void max_death(uint_fast16_t max_death) { max_death_ = max_death; };

protected:
    void execute(std::string &&execute) { execute_ = std::move(execute); }

    friend class Longrun;
    friend class Oneshot;
    // Needed for LoggerScript()
    Script() = default;

private:
    friend class bitsery::Access;
    template <typename S> void serialize(S &serializer) {
        serializer.template text<sizeof(std::string::value_type), std::string>(
            execute_, execute_.max_size());
        serializer.value2b(type_);
        serializer.ext(
            user_, bitsery::ext::StdOptional{true},
            [](S &serializer, std::string &user) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(user, user.max_size());
            });
        serializer.ext(
            user_, bitsery::ext::StdOptional{true},
            [](S &serializer, std::string &group) {
                serializer.template text<sizeof(std::string::value_type),
                                         std::string>(group, group.max_size());
            });
    }

    std::string execute_;
    Type type_;
    std::optional<std::string> user_;
    std::optional<std::string> group_;
    uint_fast32_t timeout_ = 3000;
    uint_fast32_t timeout_kill_ = 3000;
    Signal down_signal_ = Signal::kSigTerm;
    uint_fast16_t max_death_ = 3;
};

} // namespace tt

auto operator<<(std::ostream &oss, const tt::Script &script) -> std::ostream &;

#endif // TT_DATA_SCRIPT_HPP_
