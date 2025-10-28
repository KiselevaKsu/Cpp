#include "ExpressionEvaluator.h"
#include <iostream>

int main() {
    ExpressionEvaluator calc;

    try {
        std::string expr1 = "16 + 4 * (3 - 1)";
        std::cout << expr1 << " = " << calc.evaluate(expr1) << "\n";

        std::string expr2 = "2^4 + sin(90)";
        std::cout << expr2 << " = " << calc.evaluate(expr2) << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "[FATAL] " << ex.what() << "\n";
    }
}
