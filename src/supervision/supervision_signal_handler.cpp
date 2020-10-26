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

#include "tt/supervision/supervision_signal_handler.hpp"

#include <array>   // for array
#include <cassert> // for assert
#include <csignal> // for sigaction, sigaddset, SIGCHLD, SIGTERM, SIGABRT

#if __linux__
constexpr std::array<int, 23> deathsigs = {
    /* signals making us terminate */
    SIGHUP, SIGINT, SIGKILL, SIGPIPE, SIGALRM, SIGTERM, SIGUSR1, SIGUSR2,
    SIGPOLL, SIGPROF, SIGVTALRM, SIGIO, SIGPWR,
    /* signals making us coredump */
    SIGQUIT, SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGBUS, SIGSYS, SIGTRAP, SIGXCPU,
    SIGXFSZ};
#else
constexpr std::array<int, 21> deathsigs = {
    /* signals making us terminate */
    SIGHUP, SIGINT, SIGKILL, SIGPIPE, SIGALRM, SIGTERM, SIGUSR1, SIGUSR2,
    SIGPROF, SIGVTALRM, SIGIO,
    /* signals making us coredump */
    SIGQUIT, SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGBUS, SIGSYS, SIGTRAP, SIGXCPU,
    SIGXFSZ};
#endif

tt::SupervisionSignalHandler::SupervisionSignalHandler() { SetupSignals(); }

void tt::SupervisionSignalHandler::SetupSignals() {
    struct sigaction sigchld {};
    // Ignore SIGTERM while handling SIGCHLD
    sigaddset(&sigchld.sa_mask, SIGTERM);
    sigchld.sa_flags = 0;
    sigchld.sa_sigaction = SupervisionSignalHandler::staticHandleSigChld;
    sigaction(SIGCHLD, &sigchld, nullptr);

    // Handle all death signals
    for (auto signum : deathsigs) {
        struct sigaction sig {};
        sig.sa_flags = 0;
        sig.sa_handler = SupervisionSignalHandler::staticHandleSignal;
        sigaction(signum, &sig, nullptr);
    }
}

void tt::SupervisionSignalHandler::HandleSignal(int /*signum*/) {
    death_signal_received_ = true;
}

void tt::SupervisionSignalHandler::staticHandleSignal(int signum) {
    GetInstance().HandleSignal(signum);
}

void tt::SupervisionSignalHandler::staticHandleSigChld(int signo,
                                                       siginfo_t *info,
                                                       void *context) {
    GetInstance().HandleSigChld(signo, info, context);
}

void tt::SupervisionSignalHandler::HandleSigChld(int signo,
                                                 siginfo_t * /*info*/,
                                                 void * /*context*/) {
    assert(signo == SIGCHLD);
    child_exited_ = true;
}

auto tt::SupervisionSignalHandler::GetInstance() -> SupervisionSignalHandler & {
    static SupervisionSignalHandler instance;

    return instance;
}

auto tt::SupervisionSignalHandler::HasReceivedDeathSignal() -> bool {
    return GetInstance().death_signal_received_;
}

auto tt::SupervisionSignalHandler::HasChildExited() -> bool {
    return GetInstance().child_exited_;
}

void tt::SupervisionSignalHandler::ResetStatus() {
    GetInstance().child_exited_ = false;
    GetInstance().death_signal_received_ = false;
}
