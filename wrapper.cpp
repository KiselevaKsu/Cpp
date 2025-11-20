#include "wrapper.h"
#include "subject.h"

Wrapper::Wrapper(Subject* obj,
    int (Subject::* method)(int, int),
    const std::vector<std::pair<std::string, int>>& defaults)
{
    objectRef = obj;
    methodRef = method;
    defaultArgs = defaults;
}

// установка имени, чтобы engine мог по нему искать
void Wrapper::setName(const std::string& text)
{
    name = text;
}

const std::string& Wrapper::getName() const
{
    return name;
}

int Wrapper::call(const std::vector<std::pair<std::string, int>>& args)
{
    return 0;
}
