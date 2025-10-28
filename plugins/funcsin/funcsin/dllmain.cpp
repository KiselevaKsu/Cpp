#include "pch.h"
#include "FunctionInterface.h"
#include <cmath>
#include <stdexcept>

class SinFunction : public IFunction {
public:
    const char* name() const noexcept override { return "sin"; }
    int arity() const noexcept override { return 1; }

    double invoke(const double* args, int argc) override {
        if (argc != 1) throw std::runtime_error("sin() expects 1 argument");
        return std::sin(args[0] * 3.14159265358979323846 / 180.0);
    }
};

extern "C" __declspec(dllexport) IFunction* CreateFunction() {
    return new SinFunction();
}


