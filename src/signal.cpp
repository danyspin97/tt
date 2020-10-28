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

#include "tt/signal.hpp"

#include <cassert> // for assert
#include <map>     // for map, operator!=, _Rb_tree_const_iterator
#include <utility> // for pair

#include "tt/exception.hpp" // for Exception, SignalNotValidExecption

auto tt::GetSignalName(tt::Signal signal) -> std::string {
    switch (signal) {
    case Signal::kSigHup:
        return "SIGHUP";
    case Signal::kSigInt:
        return "SIGINT";
    case Signal::kSigQuit:
        return "SIGQUIT";
    case Signal::kSigKill:
        return "SIGKILL";
    case Signal::kSigTerm:
        return "SIGTERM";
    default:
        assert(false);
    }
}

auto tt::ParseSignalFromString(const std::string &signal) -> Signal {
    static const std::map<std::string, Signal> signal_strings{
        {"SIGHUP", Signal::kSigHup},   {"SIGINT", Signal::kSigInt},
        {"SIGQUIT", Signal::kSigQuit}, {"SIGKILL", Signal::kSigKill},
        {"SIGTERM", Signal::kSigTerm},
    };
    if (auto itr = signal_strings.find(signal); itr != signal_strings.end()) {
        return itr->second;
    }

    const auto error_msg = "Signal \"" + signal + "\" is not valid";
    throw tt::SignalNotValidExecption(error_msg);
}
