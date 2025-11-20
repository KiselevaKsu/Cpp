#pragma once

#include <string>
#include <vector>
#include "wrapper.h"

class Engine
{
public:
    void registerCommand(Wrapper* cmd, const std::string& text);
    int execute(const std::string& text,
        const std::vector<std::pair<std::string, int>>& args);

private:
    std::vector<Wrapper*> list;
};
