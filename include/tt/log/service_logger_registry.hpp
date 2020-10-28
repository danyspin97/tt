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

#include <memory> // for shared_ptr

#include "tt/log/logger.hpp" // for Logger

namespace tt {
class Dirs;
class LongrunLogger;
class OneshotLogger;

class ServiceLoggerRegistry {
public:
    ServiceLoggerRegistry() = delete;
    explicit ServiceLoggerRegistry(std::shared_ptr<Dirs> dirs);
    ~ServiceLoggerRegistry();

    auto GetLongrunLogger(const std::string &name) -> LongrunLogger;
    auto GetOneshotLogger(const std::string &name) -> OneshotLogger;
    auto GetScriptLoggerForService(const std::string &service_name) -> Logger;

private:
    std::shared_ptr<Dirs> dirs_;
    Logger logger_;
};

} // namespace tt
