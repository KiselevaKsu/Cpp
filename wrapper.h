#pragma once

#include <string>
#include <vector>
#include <utility>
#include <functional>

class Subject;

class Wrapper
{
public:
    Wrapper(Subject* obj,
        int (Subject::* method)(int, int),
        const std::vector<std::pair<std::string, int>>& defaults);

    void setName(const std::string& text);
    const std::string& getName() const;

    int call(const std::vector<std::pair<std::string, int>>& args);

private:
    std::string name;

    Subject* objectRef;
    int (Subject::* methodRef)(int, int);

    std::vector<std::pair<std::string, int>> defaultArgs;
};
