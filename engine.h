#pragma once

#include <string>
#include <vector>
#include "wrapper.h"

class Engine
{
public:
    template<typename Obj, typename Method, typename ArgType>
    void registerCommand(Wrapper<Obj, Method, ArgType>* cmd, const std::string& text)
    {
        cmd->setName(text);
        list.push_back((void*)cmd);
    };

    template<typename Obj, typename Method, typename ArgType>
    ArgType execute(const std::string& text,
        const std::vector<std::pair<std::string, ArgType>>& args)
    {
        for (auto* ptr : list)
        {
            auto* cmd = static_cast<Wrapper<Obj, Method, ArgType>*>(ptr);

            if (cmd->getName() == text)
                return cmd->call(args);
        }

        return ArgType{};
    }


private:
    std::vector<void*> list;
};
