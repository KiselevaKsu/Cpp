#include "pch.h"
#include "FunctionInterface.h"
#include <cmath>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class CosFunction : public IFunction {
public:
    const char* name() const noexcept override { return "cos"; }
    int arity() const noexcept override { return 1; }
    double invoke(const double* args, int argc) override {
        if (argc != 1) throw std::runtime_error("cos() expects 1 argument");
        return std::cos(args[0] * M_PI / 180.0);
    }
};

extern "C" __declspec(dllexport) IFunction* CreateFunction() {
    return new CosFunction();
}
