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

#include "tt/data/long_lived_script.hpp"                   // for LongLived...
#include "tt/data/script.hpp"                              // for Script
#include "tt/log/script_logger.hpp"                        // for ScriptLogger
#include "tt/net/client.hpp"                               // for Client
#include "tt/net/socket.hpp"                               // for Socket
#include "tt/path/dirs.hpp"                                // for Dirs
#include "tt/request/notify_service_status.hpp"            // for NotifyServ...
#include "tt/request/pack_request.hpp"                     // for PackAction
#include "tt/supervision/long_lived_script_supervisor.hpp" // for LongLived...
#include "tt/supervision/script_supervisor.hpp"            // for ScriptSup...
#include "tt/supervision/signal_handler.hpp"               // for Supervisi...

tt::LongrunSupervisor::LongrunSupervisor(Longrun &&longrun,
                                         LongrunLogger &&logger,
                                         const std::shared_ptr<Dirs>& dirs)
    : longrun_(std::move(longrun)), logger_(std::move(logger)),
      run_supervisor_(longrun_.name(), longrun_.run(), longrun_.environment(),
                      logger_.GetScriptLogger()),
      ipc_client_(dirs->livedir() / "tt-ipc.socket") {
    // Try connecting two times
    if (!ipc_client_.Connect()) {
        ipc_client_.Connect();
    }
}

auto tt::LongrunSupervisor::Run() -> bool {
    auto set = GetEmptySignalSet();
    AddSignalToSet(SIGCHLD, &set);
    // SIGCHLD is already masked in Command::SuperviseCommand

    // Every time the daemon goes down, try to restart it
    auto status = ScriptStatus::Failure;
    while (true) {
        NotifyStatus(ServiceStatus::Starting);
        status = ExecuteScript();
        // We tried running the longrun but it didn't start properly
        if (status == ScriptStatus::Failure) {
            break;
        }
        NotifyStatus(ServiceStatus::Up);
        // The longrun started, but it might fail in the future
        // Wait for sigchld signals
        WaitOnSignalSet(&set);
        // Here the run script failed (either we called Kill() from main
        // or the daemon exited)
        ExecuteFinishScript();
        if (!should_run_again_.load()) {
            break;
        }
        // Notify that the service is down, even if we are going to run it again
        // some services may need to be restarted as well
        NotifyStatus(ServiceStatus::Down);
    }
    NotifyStatus(ServiceStatus::Down);

    // Return true if the script run fine and we stopped because
    // Kill() was called
    return status == ScriptStatus::Success;
}

auto tt::LongrunSupervisor::ExecuteScript() -> ScriptStatus {
    auto time_to_try = longrun_.run().max_death();
    decltype(time_to_try) time_tried = 0;
    auto status = ScriptStatus::Failure;
    do {
        status = TryExecute();
        // Avoid starting a script when we were told to stop (by calling Kill())
    } while (should_run_again_.load() && ++time_tried < time_to_try &&
             status != ScriptStatus::Success);
    return status;
}

auto tt::LongrunSupervisor::TryExecute() -> ScriptStatus {
    if (run_supervisor_.ExecuteScript() == ScriptStatus::Success) {
        return ScriptStatus::Success;
    }

    ExecuteFinishScript();
    return ScriptStatus::Failure;
}

void tt::LongrunSupervisor::ExecuteFinishScript() const {
    if (longrun_.finish()) {
        ScriptSupervisor(longrun_.name(), longrun_.finish().value(),
                         longrun_.environment(), logger_.GetScriptLogger());
    }
}

void tt::LongrunSupervisor::NotifyStatus(ServiceStatus status) {
    if (!ipc_client_.IsConnected()) {
        return;
    }
    request::NotifyServiceStatus request(longrun_.name(), status);
    auto s = request::PackRequest(request);
    ipc_client_.SendMessage(s);
}

void tt::LongrunSupervisor::Kill() {
    should_run_again_.store(false);
    NotifyStatus(ServiceStatus::Stopping);
    run_supervisor_.Kill();
}
