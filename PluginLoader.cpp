#include "PluginLoader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using CreateFunc_t = IFunction * (__cdecl*)();

PluginLoader::~PluginLoader() {
    // все библиотеки будут освобождены позже (в main)
}

std::vector<std::unique_ptr<PluginDesc>> PluginLoader::loadAll(const std::string& folder) {
    std::vector<std::unique_ptr<PluginDesc>> result;

    try {
        if (!fs::exists(folder)) {
            std::cerr << "[INFO] Plugin folder not found: " << folder << "\n";
            return result;
        }

        for (auto& entry : fs::directory_iterator(folder)) {
            if (!entry.is_regular_file()) continue;

            auto ext = entry.path().extension().string();
            if (ext == ".dll" || ext == ".DLL") {
                std::string full = entry.path().string();
                std::unique_ptr<PluginDesc> desc = std::make_unique<PluginDesc>();
                if (tryLoadDll(full, desc)) {
                    result.push_back(std::move(desc));
                }
            }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERROR] Plugin scan failed: " << ex.what() << "\n";
    }

    return result;
}

