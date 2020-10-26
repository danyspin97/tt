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

#include "tt/log/oneshot_logger.hpp"

#include <memory> // for __shared_ptr_access

#include "spdlog/logger.h" // for logger

#include "tt/log/logger.hpp" // for Logger

void tt::OneshotLogger::Start() const {
    logger()->info("Starting oneshot {}", service_name());
}

void tt::OneshotLogger::Success() const {
    logger()->info("Done oneshot {}", service_name());
}

void tt::OneshotLogger::Fail() const {
    logger()->error("Error on oneshot {}", service_name());
}
