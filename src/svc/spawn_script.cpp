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

#include "tt/svc/spawn_script.hpp"

#include <csignal>

#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include "process.hpp"

#include "tt/script/script_builder_factory.hpp"
#include "tt/script/shell_script_builder.hpp"
#include "tt/status.hpp"
#include "tt/svc/timeout.hpp"
#include "tt/svc/utils.hpp"
#include "tt/user_dirs.hpp"

tt::SpawnScript::SpawnScript(const std::string &service_name,
                             const Script &script,
                             const Environment &environment)
    : service_name_(service_name), script_(script), environment_(environment) {
    InitLogger();
}

auto tt::SpawnScript::Spawn() -> ScriptStatus {
    auto max_death = script_.max_death();
    decltype(max_death) time_tried = 0;
    ScriptStatus status = ScriptStatus::Failure;
    do {
        status =
            TrySpawn(Timeout{std::chrono::milliseconds(script_.timeout())});
        time_tried++;
    } while (time_tried != max_death && status != ScriptStatus::Success);
    return status;
}

auto tt::SpawnScript::TrySpawn(Timeout timeout) -> ScriptStatus {
    ExecuteScript();
    while (!HasExited() && !timeout.TimedOut()) {
        std::this_thread::yield();
    }
    if (HasExited()) {
        if (GetExitStatus() != 0) {
            return ScriptStatus::Failure;
        }
        return ScriptStatus::Success;
    }

    Kill(Timeout{std::chrono::milliseconds(script_.timeout_kill())});
    return ScriptStatus::Failure;
}

void tt::SpawnScript::ExecuteScript() {
    auto builder = ScriptBuilderFactory::GetScriptBuilder(script_.type());
    auto command = builder->script(script_.execute(), environment_);
    process_ = std::make_unique<TinyProcessLib::Process>(
        command, "",
        [this](const char *bytes, size_t size) { Read(true, bytes, size); },
        [this](const char *bytes, size_t size) { Read(false, bytes, size); });
}

void tt::SpawnScript::Read(bool is_stdout, const char *bytes, size_t size) {
    std::string &last_line = is_stdout ? stdout_line_ : stderr_line_;
    std::string output{bytes, size};
    size_t index = output.find('\n');
    size_t last_index = 0;

    // If there is output remaining from the latest Read call
    // Append to it and log
    if (index != std::string::npos && !last_line.empty()) {
        // Lock is not needed because this function is called synchronously
        // from a thread in Process
        size_t count = index - last_index;
        if (count != 0) {
            fmt::format_to(std::back_inserter(last_line), "{}",
                           output.substr(last_index, count));
        }

        logger_->info("[{}] {}", is_stdout ? "stdout" : "stderr", last_line);
        last_line.clear();
        last_index = index + 1;
        index = output.find('\n', last_index);
    }

    // Parse all the remaining output
    // TODO: Optimize by sending all the output from last_index to
    // string::find_last('\n') to logger_ and the last chars to last_line
    while (index != std::string::npos) {
        logger_->info("[{}] {}", is_stdout ? "stdout" : "stderr",
                      output.substr(last_index, index - last_index));
        last_index = index + 1;
        index = output.find('\n', last_index);
    }

    if (last_index != index) {
        fmt::format_to(std::back_inserter(last_line), "{}",
                       output.substr(last_index, size - last_index));
    }
}

void tt::SpawnScript::Kill(Timeout timeout) {
    Signal(static_cast<int>(script_.down_signal()));
    while (!HasExited() && !timeout.TimedOut()) {
        std::this_thread::yield();
    }
    if (!HasExited()) {
        Signal(static_cast<int>(Signal::kSigKill));
    }
}

void tt::SpawnScript::Signal(int signum) { process_->signal(signum); }

auto tt::SpawnScript::HasExited() -> bool {
    if (exit_status_ != -1) {
        return true;
    }

    int exit_status = 0;
    if (process_->try_get_exit_status(exit_status)) {
        exit_status_ = exit_status;
        return true;
    }

    return false;
}

auto tt::SpawnScript::GetExitStatus() -> int {
    if (exit_status_ != -1) {
        return exit_status_;
    }

    return exit_status_ = process_->get_exit_status();
}

auto tt::SpawnScript::GetEnviromentFromScript() -> std::vector<const char *> {
    auto environment_vec = environment_.Vector();
    std::vector<const char *> environment_vec_cstr{environment_vec.size()};
    std::for_each(environment_vec.begin(), environment_vec.end(),
                  [&environment_vec_cstr](const std::string &key_value) {
                      environment_vec_cstr.push_back(key_value.c_str());
                  });
    return environment_vec_cstr;
}

void tt::SpawnScript::SetupUidGid() {
    // TODO: implement
}

void tt::SpawnScript::InitLogger() {
    // Check if the logger has already been added
    if ((logger_ = spdlog::get(service_name_))) {
        return;
    }

    std::filesystem::path logdir = Status::GetInstance().dirs().logdir();
    logger_ = spdlog::basic_logger_mt<spdlog::async_factory>(
        service_name_,
        logdir / std::filesystem::path{service_name_ + std::string{".log"}});

    logger_->set_pattern("{%d:%m:%Y %T} %v");
}

auto tt::SpawnScript::service_name() const -> const std::string & {
    return service_name_;
}
