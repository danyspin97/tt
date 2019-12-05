class Service {
    // Name of the service, same as the service file
    string name;
    // Description of the service
    string description;
    // User that will own the service
    // Can be 'root', 'user' or any valid user in the system
    string[] user;
    // Notify option of s6-rc
    int notify = -1;

    class Script {
        enum BuildType {
            Auto,
            Custom
        }
        BuildType build = BuildType.Auto;
        string execute;
    }
}
