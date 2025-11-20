#include "wrapper.h"

// установка имени, чтобы engine мог по нему искать
void Wrapper::setName(const std::string& text)
{
    name = text;
}

const std::string& Wrapper::getName() const
{
    return name;
}
