#include "ExpressionEvaluator.h"
#include "PluginLoader.h"
#include <iostream>
#include <cmath>

void testSin(ExpressionEvaluator& calc) {
    double result = calc.evaluate("sin(90)");
    std::cout << "sin(90) = " << result << "\n";
}

void testCos(ExpressionEvaluator& calc) {
    double result = calc.evaluate("cos(0)");
    std::cout << "cos(0) = " << result << "\n";
}

void testTan(ExpressionEvaluator& calc) {
    double result = calc.evaluate("tan(45)");
    std::cout << "tan(45) = " << result << "\n";
}

void testLn(ExpressionEvaluator& calc) {
    double result = calc.evaluate("ln(2.718281828)");
    std::cout << "ln(e) = " << result << "\n";
}

void testLnEr(ExpressionEvaluator& calc) {
    double result = calc.evaluate("ln(-1)");
    std::cout << "ln(-100) = " << result << "\n";
}

void testPowAndOperators(ExpressionEvaluator& calc) {
    double result = calc.evaluate("2^4 + 3*5 - 1");
    std::cout << "2^4 + 3*5 - 1 = " << result << "\n";
}

void testDeg(ExpressionEvaluator& calc) {
    double result = calc.evaluate("deg(3.1415926535)");
    std::cout << "deg(pi) = " << result << "\n";
}

void runAllTests(ExpressionEvaluator& calc) {
    auto safeTest = [&calc](auto func, const char* name) {
        try {
            func(calc);
        } catch (const std::exception& ex) {
            std::cerr << "[TEST ERROR] " << name << ": " << ex.what() << "\n";
        }
    };

    safeTest(testSin, "sin");
    safeTest(testCos, "cos");
    safeTest(testTan, "tan");
    safeTest(testLn, "ln");
    safeTest(testLnEr, "ln(-100)");
    safeTest(testPowAndOperators, "2^4 + 3*5 - 1");
    safeTest(testDeg, "deg");
}

int main() {
    ExpressionEvaluator calc;
    PluginLoader loader;

    loader.loadAll("./plugins");

    loader.registerAll(calc);

    runAllTests(calc);

    return 0;
}
