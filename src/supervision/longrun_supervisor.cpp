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

#include "tt/supervision/longrun_supervisor.hpp"

#include <cstdlib>      // for exit
#include <cxxabi.h>     // for __forced_...
#include <future>       // for future
#include <memory>       // for allocator
#include <optional>     // for optional
#include <string>       // for string
#include <system_error> // for system_error
#include <unistd.h>     // for pause
#include <utility>      // for move

#include "msgpack.hpp" // IWYU pragma: keep

#include "tt/action/notify_up_action.hpp"                  // for NotifyUpA...
#include "tt/action/pack_action.hpp"                       // for PackAction
#include "tt/data/long_lived_script.hpp"                   // for LongLived...
#include "tt/data/script.hpp"                              // for Script
#include "tt/log/script_logger.hpp"                        // for ScriptLogger
#include "tt/net/client.hpp"                               // for Client
#include "tt/net/socket.hpp"                               // for Socket
#include "tt/supervision/long_lived_script_supervisor.hpp" // for LongLived...
#include "tt/supervision/script_supervisor.hpp"            // for ScriptSup...
#include "tt/supervision/signal_handler.hpp"               // for Supervisi...

tt::LongrunSupervisor::LongrunSupervisor(Longrun &&longrun,
                                         LongrunLogger &&logger)
    : longrun_(std::move(longrun)), logger_(std::move(logger)) {}

auto tt::LongrunSupervisor::Run() -> bool {
    sigset_t set;
    AddSignalToSet(SIGCHLD, &set);
    MaskSignals(&set);

    // Every time the daemon goes down, try to restart it
    auto status = ScriptStatus::Failure;
    while (should_run_again_.load()) {
        status = ExecuteScript();
        NotifyStatus(status);
        // We tried running the longrun but it didn't start properly
        if (status == ScriptStatus::Failure) {
            break;
        }
        // The longrun started, but it might fail in the future
        // Wait for sigchld signals
        WaitOnSignalSet(&set);
        // Here the run script failed (either we called Kill() from main
        // or the daemon exited)
        ExecuteFinishScript();
    }

    return status == ScriptStatus::Success;
}

auto tt::LongrunSupervisor::ExecuteScript() -> ScriptStatus {
    auto time_to_try = longrun_.run().max_death();
    decltype(time_to_try) time_tried = 0;
    auto status = ScriptStatus::Failure;
    do {
        status = TryExecute();
        time_tried++;
    } while (time_tried < time_to_try && status != ScriptStatus::Success);
    return status;
}

auto tt::LongrunSupervisor::TryExecute() -> ScriptStatus {
    run_supervisor_ = std::make_unique<LongLivedScriptSupervisor>(
        longrun_.name(), longrun_.run(), longrun_.environment(),
        logger_.GetScriptLogger());

    // Avoid starting a script when we were told to stop (by calling Kill())
    if (should_run_again_.load() &&
        run_supervisor_->ExecuteScript() == ScriptStatus::Success) {
        return ScriptStatus::Success;
    }
    run_supervisor_ = nullptr;

    ExecuteFinishScript();
    return ScriptStatus::Failure;
}

void tt::LongrunSupervisor::ExecuteFinishScript() const {
    if (longrun_.finish()) {
        ScriptSupervisor(longrun_.name(), longrun_.finish().value(),
                         longrun_.environment(), logger_.GetScriptLogger());
    }
}

void tt::LongrunSupervisor::NotifyStatus(ScriptStatus status) const {
    net::Client client(net::Socket::Protocol::TCP, "localhost", 8888);
    client.Connect();

    bool succeded = status == ScriptStatus::Success;
    NotifyUpAction action(longrun_.name(), succeded);
    auto s = PackAction(action);
    client.SendMessage(s);
}

void tt::LongrunSupervisor::Kill() {
    should_run_again_.store(false);
    if (run_supervisor_) {
        run_supervisor_->Kill();
    }
}
