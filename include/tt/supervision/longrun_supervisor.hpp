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

#include "tt/data/longrun.hpp"                             // for Longrun
#include "tt/log/longrun_logger.hpp"                       // for LongrunLo...
#include "tt/supervision/long_lived_script_supervisor.hpp" // for LongLived...
#include "tt/supervision/types.hpp"                        // for ScriptStatus

namespace tt {

class LongrunSupervisor {
public:
    LongrunSupervisor(Longrun &&longrun, LongrunLogger logger);
    void ExecuteScript();

private:
    void NotifyStatus(ScriptStatus status) const;
    auto TryExecute() -> ScriptStatus;

    Longrun longrun_;
    LongrunLogger logger_;
    LongLivedScriptSupervisor spawn_;
};

} // namespace tt
