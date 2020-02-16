// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module libtt.exception;

class BooleanParseException : Exception
{
    this(string msg, string file = __FILE__, size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class BuilderException : Exception
{
    this(string msg, string file = __FILE__, size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class DummyBuilderException : Exception
{
    this(string msg, string file = __FILE__, size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class ServiceNotFoundParserException : Exception
{
    this(string msg, string file = __FILE__, size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

