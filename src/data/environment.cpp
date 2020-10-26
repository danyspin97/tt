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

#include "tt/data/environment.hpp"

#include <algorithm> // for for_each, max

auto tt::Environment::Get(const std::string &key) const -> std::string {
    for (const auto &pair : pairs_) {
        if (std::get<0>(pair) == key) {
            return std::get<1>(pair);
        }
    }

    // TODO: Should thir throw an error?
    return "";
}

void tt::Environment::Set(const std::string &key, const std::string &value) {
    for (auto &pair : pairs_) {
        if (std::get<0>(pair) == key) {
            std::get<1>(pair) = value;
            return;
        }
    }

    pairs_.emplace_back(key, value);
}

auto tt::Environment::SetUnique(const std::string &key,
                                const std::string &value) -> bool {
    for (auto &pair : pairs_) {
        if (std::get<0>(pair) == key) {
            return false;
        }
    }

    pairs_.emplace_back(key, value);
    return true;
}

auto tt::Environment::CountKeys() const -> int { return pairs_.size(); }

void tt::Environment::UpdateValuesWithEnvironment(const Environment &env) {
    for (const auto &new_pair : env.pairs_) {
        for (auto &old_pair : pairs_) {
            if (std::get<0>(old_pair) == std::get<0>(new_pair)) {
                std::get<1>(old_pair) = std::get<1>(new_pair);
            }
        }
    }
}

auto tt::Environment::Vector() const -> std::vector<std::string> {
    std::vector<std::string> vec{pairs_.size()};
    std::for_each(pairs_.begin(), pairs_.end(), [&vec](auto &pair) {
        vec.emplace_back(std::get<0>(pair) + "=" + std::get<1>(pair));
    });
    return vec;
}

auto tt::Environment::Dump(std::ostream &strm) const -> std::ostream & {
    for (const auto &pair : pairs_) {
        strm << std::get<0>(pair) << " = \"" << std::get<1>(pair) << "\"\n";
    }
    return strm;
}

auto operator<<(std::ostream &strm, const tt::Environment &env)
    -> std::ostream & {
    return env.Dump(strm);
}
