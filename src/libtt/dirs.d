module libtt.dirs;

enum Version = "@version@";

struct Dirs
{
    string bin = "@bindir@";
    string conf = "@confdir@";
    string execlinePrefix = "@execline_prefix@";
    string libexec = "@libexecdir@";
    string live = "@livedir@";
    string log = "@logdir@";
    string service = "@servicedir@";
    string system = "@systemdir@";

    string customScripts = "@confdir@";
    string adminService = "@confdir@/service";
    string adminConf = "@confdir@/conf";
}

static Dirs dirs;
