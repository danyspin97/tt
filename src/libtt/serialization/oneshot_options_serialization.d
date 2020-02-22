module libtt.serialization.oneshot_options_serialization;

@safe:
nothrow:

import libtt.data;
import libtt.serialization.service_options_serialization;

import std.conv : to;
import std.json;
import std.algorithm;
import std.array;

class OneshotOptionsSerialization : ServiceOptionsSerialization
{
public:
    this(OneshotOptions oneshotOptions)
    {
        super(oneshotOptions);
        this.oneshotOptions = oneshotOptions;
    }

    @property override JSONValue json() @system
    {
        auto j = super.json;
        j.object["optional"] = oneshotOptions.optional;

        return j;
    }

private:
    OneshotOptions oneshotOptions;
}
