import std.conv : to;
import std.file;
import std.string : toStringz ;

import libtt.dirs;
import libtt.services.service;

int parse_all_services(bool system_mode = true) {
    immutable string[] dirs_to_read = [ dirs.service, dirs.admin_service ];

    string[] services;

    foreach(dir; dirs_to_read) {
        foreach(string name; dirEntries(dir, SpanMode.shallow)) {
            services ~= name;
        }
    }
    return 0;
}

auto parse_services(string [] service_names, bool system_mode = true) {
    immutable string[] dirs_to_read = [ dirs.service, dirs.admin_service ];

    Service[] services;

    foreach(dir; dirs_to_read) {
        foreach(name; service_names) {
            auto path = dir ~ "/" ~ name;
            if (exists(path) && isFile(path)) {
                // services ~= parse_single_service(path);
            }
        }
    }
    return services;
}

void main() {
    import std.stdio;
    //parse_env("/tmp/myenv");
    // auto sys = parse_services(["mys"]);
    // foreach (s; sys) {
    //     import libtt.oneshot;
    //     auto o = cast(Oneshot)s;
    //     writeln(o.start.execute);
    // }
}
