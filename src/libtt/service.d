module libtt.service;

class Service {
    // Name of the service, same as the service file
    string name;
    // Description of the service
    string description;
    // User that will own the service
    // Can be 'root', 'user' or any valid user in the system
    string[] user;


    this() {}

    this(Service s) {
        name = s.name;
        description = s.description;
        user = s.user;
    }
}
