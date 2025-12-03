#pragma once

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include "subject.h"


template<typename Obj, typename Method>
class Wrapper
{
public:
    Wrapper(Obj* obj,
        Method method,
        const std::vector<std::pair<std::string, int>>& defaults)
        : objectRef(obj),
        methodRef(method),
        defaultArgs(defaults)
    {
    }

    void setName(const std::string& text)
    {
        name = text;
    }
    const std::string& getName() const
    {
        return name;
    }

    int call(const std::vector<std::pair<std::string, int>>& args)
    {
        int a = getValue("arg1", args);
        int b = getValue("arg2", args);

        return std::invoke(methodRef, objectRef, a, b);
    }

private:
    std::string name;

    Obj* objectRef;
    Method methodRef;

    std::vector<std::pair<std::string, int>> defaultArgs;

private:
    int getValue(const std::string& key,
        const std::vector<std::pair<std::string, int>>& args)
    {
        for (const auto& p : args)
        {
            if (p.first == key)
                return p.second;
        }

        for (const auto& p : defaultArgs)
        {
            if (p.first == key)
                return p.second;
        }

        return 0;
    }
};
