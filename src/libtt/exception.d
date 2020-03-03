// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module libtt.exception;

@safe:
nothrow:

class BooleanParseException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class BuilderException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class CodeParserNotFinishedException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class DummyBuilderException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class EmptyValueFoundWhileParsingException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class LineNotValidWhileParsingException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class ParserHasNotBeenInitialized : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class ParserIsStillParsingException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class ServiceNotFoundParserException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class SignalNotValidException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class ValueNotValidWhileParsing : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}

class WordNotValidException : Exception
{
    this(in string msg, in string file = __FILE__, in size_t line = __LINE__) @safe
    {
        super(msg, file, line);
    }
}
