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

#include "tt/cli/command/command.hpp" // for Command

namespace args {
class Subparser;
} // namespace args
namespace tt {
class ServiceNode;
class ServiceLoggerRegistry;
namespace cli {
class GlobalOptions;
} // namespace cli
} // namespace tt

namespace tt::cli {

class ServiceControlCommand : public Command {
public:
    ServiceControlCommand(args::Subparser &parser,
                          std::shared_ptr<GlobalOptions> common_options);
    ~ServiceControlCommand() override = default;

    static constexpr inline const char *name = "svc";
    static constexpr inline const char *description = "Run service control";

protected:
    auto Execute() -> int override;

private:
    void
    SpawnNode(const ServiceNode &node,
              const std::shared_ptr<ServiceLoggerRegistry> &logger_registry);
    [[nodiscard]] auto StartServices() -> int;
};

} // namespace tt::cli
