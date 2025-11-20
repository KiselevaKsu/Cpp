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

// сли€ние аргументов: пользовательские перекрывают дефолтные
static int getValue(const std::string& key,
    const std::vector<std::pair<std::string, int>>& defaults,
    const std::vector<std::pair<std::string, int>>& given)
{
    // сначала ищем среди данных пользователем
    for (const auto& p : given)
    {
        if (p.first == key)
            return p.second;
    }

    // затем среди дефолтов
    for (const auto& p : defaults)
    {
        if (p.first == key)
            return p.second;
    }

    // если ключ отсутствует Ч просто 0
    return 0;
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
    int a = getValue("arg1", defaultArgs, args);
    int b = getValue("arg2", defaultArgs, args);

    return (objectRef->*methodRef)(a, b);
}
