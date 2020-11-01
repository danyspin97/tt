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

#pragma once

#include <array>   // for array
#include <csignal> // IWYU pragma: keep for siginfo_t, sigset_t

namespace tt {

constexpr int STOP_SIGNALS_SIZE = 10;
// signals asking us terminate
constexpr std::array<int, tt::STOP_SIGNALS_SIZE> stop_signals = {
    SIGINT, SIGQUIT, SIGTERM};

void MaskSignals(sigset_t *set);
void UnmaskSignals(sigset_t *set);

void AddSignalToSet(int signal, sigset_t *set);
template <size_t N>
void AddSignalsToSet(std::array<int, N> signals, sigset_t *set) {
    for (auto signal : signals) {
        sigaddset(set, signal);
    }
}
void RemoveSignalFromSet(int signal, sigset_t *set);

auto WaitOnSignalSet(sigset_t *signals) -> siginfo_t;

constexpr auto GetStopSignals() -> std::array<int, STOP_SIGNALS_SIZE> {
    return stop_signals;
}

} // namespace tt
