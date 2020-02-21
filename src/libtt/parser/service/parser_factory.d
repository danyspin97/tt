// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.parser_factory;

import libtt.parser.service.bundle_director : BundleDirector;
import libtt.parser.service.longrun_director : LongrunDirector;
import libtt.parser.service.oneshot_director : OneshotDirector;
import libtt.parser.service.service_director : ServiceDirector;

class ParserFactory
{
public:
    static ServiceDirector getDirectorPerType(string type)
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
}
