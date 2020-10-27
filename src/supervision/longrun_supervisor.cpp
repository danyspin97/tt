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
#include <optional>     // for optional
#include <system_error> // for system_error
#include <unistd.h>     // for pause
#include <utility>      // for move

#include "msgpack.hpp"                         // IWYU pragma: keep
#include "msgpack/v1/adaptor/adaptor_base.hpp" // for operator<<

#include "tt/action/notify_up_action.hpp"                  // for NotifyUpA...
#include "tt/action/pack_action.hpp"                       // for PackAction
#include "tt/data/long_lived_script.hpp"                   // for LongLived...
#include "tt/data/script.hpp"                              // for Script
#include "tt/log/script_logger.hpp"                        // for ScriptLogger
#include "tt/net/client.hpp"                               // for Client
#include "tt/supervision/long_lived_script_supervisor.hpp" // for LongLived...
#include "tt/supervision/script_supervisor.hpp"            // for ScriptSup...
#include "tt/supervision/supervision_signal_handler.hpp"   // for Supervisi...

namespace tt {
class Dirs;
} // namespace tt

tt::LongrunSupervisor::LongrunSupervisor(Longrun &&longrun,
                                         std::shared_ptr<Dirs> dirs)
    : longrun_(std::move(longrun)), logger_(longrun_.name(), std::move(dirs)),
      spawn_(longrun_.name(), longrun_.run(), longrun_.environment(),
             logger_.GetScriptLogger()) {}

void tt::LongrunSupervisor::ExecuteScript() {
    SupervisionSignalHandler::SetupSignals();

    auto time_to_try = longrun_.run().max_death();
    decltype(time_to_try) time_tried = 0;
    auto status = ScriptStatus::Failure;
    do {
        status = TryExecute();
        time_tried++;
    } while (time_tried < time_to_try && status != ScriptStatus::Success);

    NotifyStatus(ScriptStatus::Failure);
    exit(1);
}

auto tt::LongrunSupervisor::TryExecute() -> ScriptStatus {
    SupervisionSignalHandler::ResetStatus();

    // Runs in another thread
    auto futptr = std::make_shared<std::future<void>>();
    *futptr = std::async(std::launch::async, [futptr, this]() {
        if (spawn_.ExecuteScript() == ScriptStatus::Success) {
            NotifyStatus(ScriptStatus::Success);
        }
    });

    while (true) {
        pause();

        if (SupervisionSignalHandler::HasReceivedDeathSignal()) {
            spawn_.Kill();
            NotifyStatus(ScriptStatus::Failure);
            exit(255);
        }

        if (SupervisionSignalHandler::HasChildExited()) {
            if (longrun_.finish()) {
                ScriptSupervisor(longrun_.name(), longrun_.finish().value(),
                                 longrun_.environment(),
                                 logger_.GetScriptLogger());
            }
            return ScriptStatus::Failure;
        }
    }
}

void tt::LongrunSupervisor::NotifyStatus(ScriptStatus status) const {
    net::Client client(net::Socket::Protocol::TCP, "localhost", 8000);
    client.Connect();

    bool succeded = status == ScriptStatus::Success;
    NotifyUpAction action(longrun_.name(), succeded);
    auto s = PackAction(action);
    client.SendMessage(s);
}
