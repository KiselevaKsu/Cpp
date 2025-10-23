#include "Parser.h"
#include <iostream>

int main() {
    Parser parser("16 + 4 * (3 - 1)");
    auto rpn = parser.parse();

    std::cout << "RPN output:\n";
    for (auto& item : rpn) {
        std::cout << item.text << " ";
    }
    std::cout << "\n";
}
