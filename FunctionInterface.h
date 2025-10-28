#pragma once
#include <cstddef>

// реализует каждая функция в плагине
class IFunction {
public:
    virtual const char* name() const noexcept = 0;

    // количество аргументов или -1 для переменного числа аргументов
    virtual int arity() const noexcept = 0;

    virtual double invoke(const double* args, int argc) = 0;

    virtual ~IFunction() = default;
};
