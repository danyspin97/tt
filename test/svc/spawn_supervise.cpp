
#include "tt/svc/spawn_supervise.hpp"
#include "tt/svc/types.hpp"

#include "catch2/catch.hpp"

#include <unistd.h>

#include <filesystem>

TEST_CASE("SpawnScript") {
    SECTION("Spawn sucessfull script") {
        std::filesystem::path testfile{"/tmp/tt"};
        if (std::filesystem::exists(testfile)) {
            std::filesystem::remove(testfile);
        }

        tt::PipeFd pipe1;
        tt::PipeFd pipe2;
        pipe(pipe1.data());
        pipe(pipe2.data());

        tt::PipeFd supervisor_fd{pipe1.at(0), pipe2.at(1)};
        // tt::PipeFd process_fd{pipe2.at(0), pipe1.at(1)};
        tt::SpawnSupervise supervise(supervisor_fd);

        if (int pid = fork(); pid == 0) {
            std::vector<char *> args{};
            std::vector<const char *> env{};
            env.push_back("PATH=/bin:/usr/bin");
            env.push_back("SHELL=/bin/bash");
            env.push_back(0);
            args.push_back(const_cast<char *>("touch"));
            args.push_back(const_cast<char *>(testfile.c_str()));
            supervise.Spawn(args, env);
        } else {
            // Wait for the file to be created
            sleep(1);
            CHECK(std::filesystem::exists(testfile));
        }
    }
}
