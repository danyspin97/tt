
#include "tt/user_dirs.hpp"

#include <cassert>

#include "xdg.hpp"

tt::UserDirs::UserDirs() {
    // This exe has not been run as root
    assert(geteuid() > 0);

    xdg::BaseDirectories dirs{};
    std::optional<std::filesystem::path> runtime_dir = dirs.Runtime();
    if (runtime_dir) {
        livedir(runtime_dir.value() / "tt");
    } else {
        livedir(dirs.CacheHome() / "tt");
    }

    confdir(dirs.ConfigHome() / "tt");
    statedir(dirs.DataHome() / "tt" / "state");
    logdir(dirs.DataHome() / "tt" / "log");
}
