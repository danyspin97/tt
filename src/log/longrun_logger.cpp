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

#include "tt/log/longrun_logger.hpp"

#include <memory> // for __shared_ptr_access

#include "spdlog/logger.h" // for logger

#include "tt/log/logger.hpp" // for Logger

void tt::LongrunLogger::Start() const {
    logger()->info("Starting longrun {}", service_name());
}

void tt::LongrunLogger::Success() const {
    logger()->info("Longrun {} has started", service_name());
}

void tt::LongrunLogger::Fail() const {
    logger()->error("Error on longrun {}", service_name());
}
