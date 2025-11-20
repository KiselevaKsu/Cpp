#include "engine.h"

// просто сохраняем указатель и имя
void Engine::registerCommand(Wrapper* cmd, const std::string& text)
{
    cmd->setName(text);
    list.push_back(cmd);
}

int Engine::execute(const std::string& text,
    const std::vector<std::pair<std::string, int>>& args)
{
    // ищем команду по имени
    for (auto* cmd : list)
    {
        if (cmd->getName() == text)
        {
            // вызываем метод через wrapper
            return cmd->call(args);
        }
    }

    // если команда не найдена — пусть будет 0
    return 0;
}
