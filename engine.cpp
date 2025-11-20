#include "engine.h"

// просто сохраняем указатель
void Engine::registerCommand(Wrapper* cmd, const std::string& text)
{
    cmd->setName(text);
    list.push_back(cmd);
}

int Engine::execute(const std::string& text,
    const std::vector<std::pair<std::string, int>>& args)
{
    // заглушка
    // потом тут будет вызов wrapper
    return 0;
}
