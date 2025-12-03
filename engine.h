#pragma once

#include <string>
#include <vector>
#include "wrapper.h"

class Engine
{
public:
    template<typename Obj, typename Method>
    void registerCommand(Wrapper<Obj, Method>* cmd, const std::string& text)
    {
        cmd->setName(text);
        list.push_back((void*)cmd);
    };

    template<typename Obj, typename Method>
    int execute(const std::string& text,
        const std::vector<std::pair<std::string, int>>& args) 
    {
        for (auto* ptr : list)
        {
            auto* cmd = static_cast<Wrapper<Obj, Method>*>(ptr);

            if (cmd->getName() == text)
                return cmd->call(args);
        }

        return 0;
    };

private:
    std::vector<void*> list;
};
