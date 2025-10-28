#pragma once
#include <functional>
#include <string>

// описание функции, которую экспортирует плагин
// каждый плагин должен реализовать extern "C" функцию getFunction()

struct PluginFunction {
    std::string name;
    std::function<double(const double*, int)> caller;
    int arity;
};

// сигнатура функции, которую должен экспортировать каждый плагин
extern "C" __declspec(dllexport)
PluginFunction getFunction();
