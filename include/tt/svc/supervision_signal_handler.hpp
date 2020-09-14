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

#pragma once

#include <csignal>

namespace tt {

class SupervisionSignalHandler {
public:
    SupervisionSignalHandler();
    static void SetupSignals();

    void HandleSignal(int signum);
    static void staticHandleSignal(int signum);

    void HandleSigChld(int signo, siginfo_t *info, void *context);
    static void staticHandleSigChld(int signo, siginfo_t *info, void *context);

    static auto GetInstance() -> SupervisionSignalHandler &;

    static auto HasReceivedDeathSignal() -> bool;
    static auto HasChildExited() -> bool;

    static void ResetStatus();

private:
    bool death_signal_received_ = false;
    bool child_exited_ = false;
};

} // namespace tt
