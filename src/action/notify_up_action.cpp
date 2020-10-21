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

#include "tt/action/notify_up_action.hpp"

#include "tt/svc/service_status.hpp"
#include "tt/svc/service_status_manager.hpp"

tt::NotifyUpAction::NotifyUpAction(std::string service, bool succeded)
    : service_(std::move(service)), succeeded_(succeded) {}

auto tt::NotifyUpAction::service() const -> std::string { return service_; }

auto tt::NotifyUpAction::succeded() const -> bool { return succeeded_; }

void tt::NotifyUpAction::Apply() {
    auto service_status_manager = tt::ServiceStatusManager::GetInstance();
    service_status_manager.ServiceStartUpdate(service_, succeded());
}
