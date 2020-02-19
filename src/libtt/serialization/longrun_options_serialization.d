module libtt.serialization.longrun_options_serialization;

import libtt.data;
import libtt.serialization.service_options_serialization;

import std.conv : to;
import std.json;
import std.algorithm;
import std.array;

class LongrunOptionsSerialization: ServiceOptionsSerialization
{
    public:
        this(
            LongrunOptions longrunOptions
        ){
            super(longrunOptions);
            this.longrunOptions = longrunOptions;
        }

        @property override JSONValue json()
        {
            auto j = super.json;
            j.object["notify"] = longrunOptions.notify;
            j.object["timeoutFinish"] = longrunOptions.timeoutFinish;
            j.object["timeoutKill"] = longrunOptions.timeoutKill;
            j.object["downSignal"] = longrunOptions.downSignal;
            j.object["maxDeath"] = longrunOptions.maxDeath;
            j.object["writeMessage"] = longrunOptions.writeMessage;
            j.object["optional"] = longrunOptions.optional;

            return j;
        }

    private:
        LongrunOptions longrunOptions;
}
