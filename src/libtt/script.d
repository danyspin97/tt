module libtt.script;

enum BuildType {
    Auto,
    Custom
}

class Script {
    BuildType build = BuildType.Auto;
    // Script to execute
    string execute;
    // shebang for she script
    string shebang;
    // run as the user contained in "runas"
    string runas;

    bool is_parsing;
}

