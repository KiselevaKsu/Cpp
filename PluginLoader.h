#pragma once
#include "FunctionInterface.h"
#include "ExpressionEvaluator.h"
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

    const std::vector<std::unique_ptr<PluginDesc>>& getLoaded() const { return loaded_; }

    void loadAll(const std::string& folder = "./plugins");
    void registerAll(ExpressionEvaluator& eval);

private:
    std::vector<std::unique_ptr<PluginDesc>> loaded_;
    bool tryLoadDll(const std::string& fullpath, std::unique_ptr<PluginDesc>& out);
};
