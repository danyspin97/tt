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

#include "tt/supervisor/supervisor.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

tt::Supervisor::Supervisor(PipeFd supervisor_fd)
    : supervisor_fd_(supervisor_fd) {
    Supervise();
}

void tt::Supervisor::Supervise() {
    // The parent should run supervise executable
    // The children should follow the code and let Spawn*Script run the script
    if (int pid = fork(); pid != 0) {
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        dup2(supervisor_fd_.at(0), STDIN_FILENO);
        dup2(supervisor_fd_.at(1), STDOUT_FILENO);
        std::vector<char *> args{};
        args.push_back(const_cast<char *>("supervise"));
        std::vector<const char *> environment{};
        execve("/bin/supervise", const_cast<char **>(args.data()),
               const_cast<char *const *>(environment.data()));
    }
}
