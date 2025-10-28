#include "PluginLoader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using CreateFunc_t = IFunction * (__cdecl*)();

PluginLoader::~PluginLoader() {
    for (auto& desc : loaded_) {
        if (desc) {
            if (desc->funcPtr) {
                delete desc->funcPtr; // удаляем объект функции
                desc->funcPtr = nullptr;
            }
            if (desc->module) {
                FreeLibrary(desc->module);  // выгружаем DLL
                desc->module = nullptr;
            }
        }
    }
    loaded_.clear();
    std::cout << "[DEBUG] All plugins unloaded.\n";
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

bool PluginLoader::tryLoadDll(const std::string& fullpath, std::unique_ptr<PluginDesc>& out) {
    HMODULE h = LoadLibraryA(fullpath.c_str());
    if (!h) {
        std::cerr << "[ERROR] Failed to load DLL: " << fullpath << "\n";
        return false;
    }

    auto proc = reinterpret_cast<CreateFunc_t>(GetProcAddress(h, "CreateFunction"));
    if (!proc) {
        std::cerr << "[WARN] DLL does not export CreateFunction(): " << fullpath << "\n";
        FreeLibrary(h);
        return false;
    }

    IFunction* f = nullptr;
    try {
        f = proc();
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERROR] Exception in factory from " << fullpath << ": " << ex.what() << "\n";
        FreeLibrary(h);
        return false;
    }
    catch (...) {
        std::cerr << "[ERROR] Unknown exception in factory from " << fullpath << "\n";
        FreeLibrary(h);
        return false;
    }

    if (!f) {
        std::cerr << "[WARN] Factory returned null in " << fullpath << "\n";
        FreeLibrary(h);
        return false;
    }

    out->path = fullpath;
    out->module = h;
    out->funcPtr = f;
    std::cout << "[OK] Loaded plugin: " << fullpath << " (function: " << f->name() << ")\n";
    return true;
}

void PluginLoader::registerAll(ExpressionEvaluator& eval, const std::vector<std::unique_ptr<PluginDesc>>& plugins) {
    for (const auto& p : plugins) {
        if (!p || !p->funcPtr) continue;

        IFunction* f = p->funcPtr;
        try {
            // адаптируем IFunction к std::function<double(const double*, int)>
            eval.registerFunction(
                f->name(),
                [f](const double* args, int argc) {
                    return f->invoke(args, argc);
                },
                f->arity()
            );

            std::cout << "[OK] Registered plugin function: " << f->name() << "\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "[ERROR] Failed to register function " << f->name() << ": " << ex.what() << "\n";
        }
    }
}
