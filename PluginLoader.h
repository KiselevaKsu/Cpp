#pragma once
#include "FunctionInterface.h"
#include <string>
#include <vector>
#include <memory>
#include <windows.h>

struct PluginDesc {
    std::string path;   // путь до DLL
    HMODULE module = nullptr;
    IFunction* funcPtr = nullptr; // указатель на созданную функцию
};

// для загрузки всех плагинов
class PluginLoader {
public:
    PluginLoader() = default;
    ~PluginLoader();

    std::vector<std::unique_ptr<PluginDesc>> loadAll(const std::string& folder = "./plugins");

};
