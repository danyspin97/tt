// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.parser_factory;

import libtt.parser.bundle_director : BundleDirector;
import libtt.parser.longrun_director : LongrunDirector;
import libtt.parser.oneshot_director : OneshotDirector;
import libtt.parser.service_director : ServiceDirector;

class ParserFactory
{
public:
    @property ServiceDirector director() { return m_director; }
    this(string type)
    {
        m_director = dispatchDirectorPerType(type);
    }

private:
    ServiceDirector dispatchDirectorPerType(string type)
    {
        switch (type)
        {
            case "bundle":
                return new BundleDirector();
            case "longrun":
                return new LongrunDirector();
            case "oneshot":
                return new OneshotDirector();
            default:
                auto msg = `Type "` ~ type ~ `" is not supported.`;
                throw new Exception(msg);
        }
    }

    ServiceDirector m_director;
}
