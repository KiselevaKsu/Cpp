#include "ExpressionEvaluator.h"
#include "Parser.h"
#include <stack>
#include <iostream>
#include <stdexcept>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

ExpressionEvaluator::ExpressionEvaluator() {
    registerFunction("sin", [](const double* a, int n) {
        if (n != 1) throw std::runtime_error("sin() expects 1 argument");
        return std::sin(a[0] * M_PI / 180.0); // градусы
        }, 1);

    registerFunction("cos", [](const double* a, int n) {
        if (n != 1) throw std::runtime_error("cos() expects 1 argument");
        return std::cos(a[0] * M_PI / 180.0);
        }, 1);

    registerFunction("tan", [](const double* a, int n) {
        if (n != 1) throw std::runtime_error("tan() expects 1 argument");
        return std::tan(a[0] * M_PI / 180.0);
        }, 1);

    registerFunction("ln", [](const double* a, int n) {
        if (n != 1) throw std::runtime_error("ln() expects 1 argument");
        if (a[0] <= 0) throw std::runtime_error("ln() domain error");
        return std::log(a[0]);
        }, 1);
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
    Parser parser(expr);
    auto rpn = parser.parse();
    std::stack<double> st;

    try {
        for (auto& item : rpn) {
            if (item.type == TokenType::Number) {
                st.push(item.value);
            }
            else if (item.type == TokenType::Operator) {
                if (st.size() < 2) throw std::runtime_error("Operator stack underflow");
                double b = st.top(); st.pop();
                double a = st.top(); st.pop();

                if (item.text == "+") st.push(a + b);
                else if (item.text == "-") st.push(a - b);
                else if (item.text == "*") st.push(a * b);
                else if (item.text == "/") {
                    if (b == 0) throw std::runtime_error("Division by zero");
                    st.push(a / b);
                }
                else if (item.text == "^") st.push(std::pow(a, b));
                else throw std::runtime_error("Unknown operator: " + item.text);
            }
            else if (item.type == TokenType::Identifier) {
                // ищем зарегистрированную функцию
                std::string key = item.text;
                for (auto& c : key) c = std::tolower(static_cast<unsigned char>(c));

                auto it = funcs_.find(key);
                if (it == funcs_.end())
                    throw std::runtime_error("Unknown function: " + item.text);

                int arity = it->second.arity;
                if (st.size() < static_cast<size_t>(arity))
                    throw std::runtime_error("Not enough arguments for " + item.text);

                double args[8]; // ограничение на кол-во аргументов
                for (int i = arity - 1; i >= 0; --i) {
                    args[i] = st.top();
                    st.pop();
                }

                double res = it->second.caller(args, arity);
                st.push(res);
            }
        }

        if (st.size() != 1)
            throw std::runtime_error("Invalid expression state");

        return st.top();
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERROR] " << ex.what() << "\n";
        throw;
    }
}
