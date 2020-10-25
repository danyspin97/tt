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

#include <future>

#include <unistd.h>

// This should be included before req0.h
#include "nngpp/socket.h"

#include "nngpp/protocol/req0.h"

#include "tt/action/notify_up_action.hpp"
#include "tt/action/pack_action.hpp"
#include "tt/supervision/long_lived_script_supervisor.hpp"
#include "tt/supervision/script_supervisor.hpp"
#include "tt/supervision/supervision_signal_handler.hpp"
#include "tt/supervision/timeout.hpp"

tt::LongrunSupervisor::LongrunSupervisor(Longrun &&longrun,
                                         std::shared_ptr<Dirs> dirs)
    : longrun_(std::move(longrun)), logger_(longrun_.name(), dirs),
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
        continue;
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
    nng::socket socket = nng::req::open();
    socket.dial("tcp://localhost:8000");

    bool succeded = status == ScriptStatus::Success;
    NotifyUpAction action(longrun_.name(), succeded);
    socket.send(PackAction(action));
}
