#pragma once
#ifndef MATH_EXAM_HPP
#define MATH_EXAM_HPP

#include <vector>
#include <string>
#include <map>

class QuadraticEquation {
public:
    double a, b, c;
    QuadraticEquation(double a = 0, double b = 0, double c = 0);
    std::string toString() const;
};

class Student {
public:
    std::vector<double> solveEquation(const QuadraticEquation& eq) const;
};

class Teacher {
public:
    void checkMails();
};

std::vector<QuadraticEquation> readEquations(const std::string& filename);

#endif