class Service {
    // Name of the service, same as the service file
    string name;
    // Description of the service
    string description;
    // User that will own the service
    // Can be 'root', 'user' or any valid user in the system
    string[] user;

    class Script {
        enum BuildType {
            Auto,
            Custom
        }
        BuildType build = BuildType.Auto;
        // Script to execute
        string execute;
        // shebang for she script
        string shebang;
        // run as the user contained in "runas"
        string runas;
    }

    this() {}

    this(Service s) {
        name = s.name;
        description = s.description;
        user = s.user;
    }
}
