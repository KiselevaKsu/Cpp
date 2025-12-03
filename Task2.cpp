#include <iostream>
#include "subject.h"
#include "wrapper.h"
#include "engine.h"

void printTest(const std::string& name, double result, double expected)
{
    std::cout << name << ": result = " << result
        << ", expected = " << expected << std::endl;
}

void testDefault()
{
    Subject subj;

    Wrapper<Subject, int(Subject::*)(int, int), int> wrapper(
        &subj,
        &Subject::f3,
        { {"arg1", 0}, {"arg2", 0} }
    );

    Engine engine;
    engine.registerCommand<Subject, int(Subject::*)(int, int), int>(&wrapper, "sum");

    int r = engine.execute<Subject, int(Subject::*)(int, int), int>(
        "sum",
        { {"arg1", 4}, {"arg2", 5} }
    );

    printTest("testOriginal", r, 9);
}

// тест double + переопределение одного аргумента
void testDoublePartialOverride()
{
    Subject subj;

    Wrapper<Subject, double(Subject::*)(double, double), double> wrapper(
        &subj,
        &Subject::f4,
        { {"arg1", 2.0}, {"arg2", 1.5} }
    );

    Engine engine;
    engine.registerCommand<Subject, double(Subject::*)(double, double), double>(&wrapper, "mul");

    double r = engine.execute<Subject, double(Subject::*)(double, double), double>(
        "mul",
        { {"arg1", 3.0} }
    );

    printTest("testDoublePartialOverride", r, 4.5);
}


int main()
{
    testDefault();
    testDoublePartialOverride();
    return 0;
}
