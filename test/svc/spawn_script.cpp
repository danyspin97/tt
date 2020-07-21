
#include "tt/supervisor/spawn_script.hpp"
#include "tt/supervisor/types.hpp"

#include "catch2/catch.hpp"

#include <unistd.h>

TEST_CASE("SpawnScript") {
    SECTION("Spawn sucessfull script") {
        tt::Environment env;
        tt::Script script{tt::Script::Type::Bash, "echo tt"};
        tt::SpawnScript spawn_script(script, env, [](std::string) {});
        tt::ScriptStatus status = spawn_script.Spawn();
        CHECK(status == tt::ScriptStatus::Success);
    }
}
