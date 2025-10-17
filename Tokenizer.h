#pragma once
#include <string>
#include <vector>
#include <cctype>

enum class TokenType {
    Number,
    Operator,
    LeftParen,
    RightParen,
    Identifier,
    End
};

struct Token {
    TokenType type;
    std::string text;
    double value = 0.0; // используется, если это число
};

// разбиениt строки на токены
class Tokenizer {
public:
    explicit Tokenizer(const std::string& input);
    Token next();

private:
    std::string expr_;
    size_t pos_ = 0;

    void skipSpaces();
    bool isEnd() const;
};
