#pragma once
#include "Tokenizer.h"
#include <vector>
#include <string>
#include <unordered_map>

struct RpnItem {
    TokenType type;
    std::string text;
    double value = 0.0;
};

// класс для преобразования токенов в обратную запись
class Parser {
public:
    explicit Parser(const std::string& expression);
    std::vector<RpnItem> parse();

private:
    Tokenizer tokenizer_;
    Token current_;

    void next();
    int precedence(const std::string& op) const;
    bool isRightAssociative(const std::string& op) const;
};
