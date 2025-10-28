#include "PluginLoader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using CreateFunc_t = IFunction * (__cdecl*)();

PluginLoader::~PluginLoader() {
    for (auto& desc : loaded_) {
        if (desc) {
            delete desc->funcPtr;
            if (desc->module) FreeLibrary(desc->module);
        }
    }
    loaded_.clear();
}

void PluginLoader::loadAll(const std::string& folder) {
    loaded_.clear();

    if (!fs::exists(folder)) {
        std::cerr << "[INFO] Plugin folder not found: " << folder << "\n";
        return;
    }

    for (auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        auto ext = entry.path().extension().string();
        if (ext == ".dll" || ext == ".DLL") {
            auto desc = std::make_unique<PluginDesc>();
            if (tryLoadDll(entry.path().string(), desc)) {
                loaded_.push_back(std::move(desc));
            }
        }
    }
}

void PluginLoader::registerAll(ExpressionEvaluator& eval) {
    for (const auto& p : loaded_) {
        if (!p || !p->funcPtr) continue;
        IFunction* f = p->funcPtr;
        try {
            eval.registerFunction(
                f->name(),
                [f](const double* args, int argc) { return f->invoke(args, argc); },
                f->arity()
            );
        }
        catch (const std::exception& ex) {
            std::cerr << "[ERROR] Failed to register function " << f->name() << ": " << ex.what() << "\n";
        }
    }
}

bool PluginLoader::tryLoadDll(const std::string& fullpath, std::unique_ptr<PluginDesc>& out) {
    HMODULE h = LoadLibraryA(fullpath.c_str());
    if (!h) return false;
    auto proc = reinterpret_cast<CreateFunc_t>(GetProcAddress(h, "CreateFunction"));
    if (!proc) { FreeLibrary(h); return false; }

    IFunction* f = nullptr;
    try { f = proc(); }
    catch (...) { FreeLibrary(h); return false; }
    if (!f) { FreeLibrary(h); return false; }

    out->path = fullpath;
    out->module = h;
    out->funcPtr = f;
    return true;
}
