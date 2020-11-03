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

#include "tt/supervision/signal_handler.hpp"

#include <array>   // for array
#include <cassert> // for assert
#include <csignal> // for sigaction, sigaddset, sigdelset

void tt::MaskSignals(sigset_t *set) {
    // TODO: Handle errors
    pthread_sigmask(SIG_BLOCK, set, nullptr);
}

void tt::UnmaskSignals(sigset_t *set) {
    // TODO: Handle errors
    pthread_sigmask(SIG_UNBLOCK, set, nullptr);
}

auto tt::GetEmptySignalSet() -> sigset_t {
    sigset_t set;
    sigemptyset(&set);
    return set;
}

auto tt::WaitOnSignalSet(sigset_t *signals) -> siginfo_t {
    siginfo_t signal_received;
    sigwaitinfo(signals, &signal_received);
    return signal_received;
}

void tt::AddSignalToSet(int signal, sigset_t *set) { sigaddset(set, signal); }

void tt::RemoveSignalFromSet(int signal, sigset_t *set) {
    sigdelset(set, signal);
}
