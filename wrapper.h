#pragma once

#include <string>
#include <vector>
#include <utility>

class Wrapper
{
public:

    Wrapper() = default;

    void setName(const std::string& text);
    const std::string& getName() const;

private:
    std::string name;
};
