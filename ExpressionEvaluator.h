#pragma once
#include "FunctionInterface.h"
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

struct FunctionEntry {
    std::function<double(const double*, int)> caller;
    int arity;
};

class ExpressionEvaluator {
public:
    ExpressionEvaluator();

    void registerFunction(const std::string& name,
        std::function<double(const double*, int)> caller,
        int arity);

    double evaluate(const std::string& expr);

private:
    std::unordered_map<std::string, FunctionEntry> funcs_;
};
