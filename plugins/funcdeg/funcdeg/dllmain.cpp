#include "pch.h"
#include "FunctionInterface.h"
#include <cmath>
#include <stdexcept>

class DegFunction : public IFunction {
public:
    const char* name() const noexcept override { return "deg"; }
    int arity() const noexcept override { return 1; }

    double invoke(const double* args, int argc) override {
        if (argc != 1) throw std::runtime_error("deg() expects 1 argument");
        return args[0] * 180.0 / 3.14159265358979323846;
    }
};

extern "C" __declspec(dllexport) IFunction* CreateFunction() {
    return new DegFunction();
}


