module libtt.services.environment;

class Environment
{
public:
    const(string) get(string key)
    {
        return env[key];
    }

    ref const(string[string]) getAll()
    {
        return env;
    }

    void set(string key, string value)
    {
        env[key] = value;
    }

    bool setUnique(string key, string value)
    {
        if (key in env)
        {
            return false;
        }

        env[key] = value;
        return true;
    }
private:
    string[string] env;
}

