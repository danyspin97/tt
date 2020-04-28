
#pragma once

namespace tt {

class Oneshot;
class Bundle;
class Longrun;

class ServiceVisitor {
public:
    virtual void Visit(Oneshot *oneshot) = 0;
    virtual void Visit(Bundle *bundle) = 0;
    virtual void Visit(Longrun *longrun) = 0;
    virtual ~ServiceVisitor() = default;
};

} // namespace tt
