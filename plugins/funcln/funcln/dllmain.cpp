#include "pch.h"
#include "FunctionInterface.h"
#include <cmath>
#include <stdexcept>

class LnFunction : public IFunction {
public:
    const char* name() const noexcept override { return "ln"; }
    int arity() const noexcept override { return 1; }
    double invoke(const double* args, int argc) override {
        if (argc != 1) throw std::runtime_error("ln() expects 1 argument");
        if (args[0] <= 0) throw std::runtime_error("ln() domain error");
        return std::log(args[0]);
    }
};

extern "C" __declspec(dllexport) IFunction* CreateFunction() {
    return new LnFunction();
}
