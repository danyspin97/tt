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

#include "libtt/signal.hpp"

#include "libtt/exception.hpp"

using std::map;
using std::string;

using tt::Signal;
using tt::SignalNotValidExecption;

Signal tt::ParseSignalFromString(const string signal) {
    static const map<string, Signal> signalStrings{
        {"SIGHUP", Signal::kSigHup},   {"SIGINT", Signal::kSigInt},
        {"SIGQUIT", Signal::kSigQuit}, {"SIGKILL", Signal::kSigKill},
        {"SIGTERM", Signal::kSigTerm},
    };
    if (auto itr = signalStrings.find(signal); itr != signalStrings.end()) {
        return itr->second;
    }

    const auto error_msg = "Signal \"" + signal + "\" is not valid";
    throw SignalNotValidExecption(error_msg);
}
