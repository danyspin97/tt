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

#include "tt/svc/supervise_longrun.hpp"

#include <unistd.h>

#include "tt/svc/spawn_long_lived_script.hpp"
#include "tt/svc/spawn_script.hpp"
#include "tt/svc/supervision_signal_handler.hpp"

tt::SuperviseLongrun::SuperviseLongrun(Longrun &&longrun)
    : longrun_(std::move(longrun)),
      spawn_(longrun_.name(), longrun_.run(), longrun_.environment()) {}

void tt::SuperviseLongrun::Spawn() {
    SupervisionSignalHandler::SetupSignals();

    auto time_to_try = longrun_.run().max_death();
    decltype(time_to_try) time_tried = 0;
    while (time_tried < time_to_try) {
        if (TrySpawn() == ScriptStatus::Failure) {
            time_tried++;
        }
        // Should never reach here
        assert(false);
    }
}

auto tt::SuperviseLongrun::TrySpawn() -> ScriptStatus {
    SupervisionSignalHandler::ResetStatus();

    // Runs in another thread
    if (int pid = fork(); pid == 0) {
        spawn_.Spawn();
    }

    pause();

    if (SupervisionSignalHandler::HasReceivedDeathSignal()) {
        spawn_.Kill();
        NotifyStatus(ScriptStatus::Failure);
        exit(255);
    }

    if (SupervisionSignalHandler::HasChildExited()) {
        if (longrun_.finish()) {
            SpawnScript(longrun_.name(), longrun_.finish().value(),
                        longrun_.environment());
        }
        return ScriptStatus::Failure;
    }

    // Should never reach here
    assert(false);
}

void tt::SuperviseLongrun::SetupLogger() {}

void tt::SuperviseLongrun::NotifyStatus(ScriptStatus /*status*/) {}
