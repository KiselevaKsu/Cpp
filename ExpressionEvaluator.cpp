#include "ExpressionEvaluator.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

ExpressionEvaluator::ExpressionEvaluator() {
}

void ExpressionEvaluator::registerFunction(const std::string& name,
    std::function<double(const double*, int)> caller,
    int arity)
{
    std::string key = name;
    for (auto& c : key) c = std::tolower(static_cast<unsigned char>(c));
    funcs_[key] = FunctionEntry{ std::move(caller), arity };
}

double ExpressionEvaluator::evaluate(const std::string& expr) {
    std::cout << "[DEBUG] ExpressionEvaluator::evaluate() called with: " << expr << "\n";
    return 0.0;
}
