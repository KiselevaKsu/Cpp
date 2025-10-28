#include "ExpressionEvaluator.h"
#include "PluginLoader.h"
#include <iostream>

int main() {
    ExpressionEvaluator calc;
    PluginLoader loader;

    auto plugins = loader.loadAll("./plugins");
    loader.registerAll(calc, plugins);

    try {
        std::cout << "2^4 + sin(90) = " << calc.evaluate("2^4 + sin(90)") << "\n";
        std::cout << "deg(3.1415926535) = " << calc.evaluate("deg(3.1415926535)") << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "[FATAL] " << ex.what() << "\n";
    }

    return 0;
}
