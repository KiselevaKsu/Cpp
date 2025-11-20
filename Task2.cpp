#include <iostream>
#include "subject.h"
#include "wrapper.h"
#include "engine.h"

int main()
{
    Subject subj;

    Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });

    Engine engine;

    engine.registerCommand(&wrapper, "command1");

    int result = engine.execute("command1", { {"arg1", 4}, {"arg2", 5} });

    std::cout << "result: " << result << std::endl;

    return 0;
}
