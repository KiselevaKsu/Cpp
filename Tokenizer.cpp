#include "Tokenizer.h"
#include <stdexcept>

Tokenizer::Tokenizer(const std::string& input)
    : expr_(input), pos_(0) {}

void Tokenizer::skipSpaces() {
    while (pos_ < expr_.size() && std::isspace(static_cast<unsigned char>(expr_[pos_]))) {
        ++pos_;
    }
}

bool Tokenizer::isEnd() const {
    return pos_ >= expr_.size();
}

Token Tokenizer::next() {
    skipSpaces();

    if (isEnd()) return { TokenType::End, "" };

    char c = expr_[pos_];

    // число (включая знак + или -)
    if ((c == '-' || c == '+') &&
        (pos_ == 0 || expr_[pos_ - 1] == '(') &&
        pos_ + 1 < expr_.size() && std::isdigit(expr_[pos_ + 1]))
    {
        size_t start = pos_;
        ++pos_;
        while (pos_ < expr_.size() && (std::isdigit(expr_[pos_]) || expr_[pos_] == '.')) ++pos_;
        Token t;
        t.type = TokenType::Number;
        t.text = expr_.substr(start, pos_ - start);
        t.value = std::stod(t.text);
        return t;
    }

    // число
    if (std::isdigit(static_cast<unsigned char>(c)) || (c == '.' && pos_ + 1 < expr_.size() && std::isdigit(expr_[pos_ + 1]))) {
        size_t start = pos_;
        while (pos_ < expr_.size() && (std::isdigit(static_cast<unsigned char>(expr_[pos_])) || expr_[pos_] == '.'))
            ++pos_;
        Token t;
        t.type = TokenType::Number;
        t.text = expr_.substr(start, pos_ - start);
        t.value = std::stod(t.text);
        return t;
    }


    // идентификатор (функция)
    if (std::isalpha(static_cast<unsigned char>(c))) {
        size_t start = pos_;
        while (pos_ < expr_.size() && (std::isalnum(static_cast<unsigned char>(expr_[pos_])) || expr_[pos_] == '_'))
            ++pos_;
        Token t;
        t.type = TokenType::Identifier;
        t.text = expr_.substr(start, pos_ - start);
        return t;
    }

    // скобки
    if (c == '(') {
        ++pos_;
        return { TokenType::LeftParen, "(" };
    }
    if (c == ')') {
        ++pos_;
        return { TokenType::RightParen, ")" };
    }

    // операторы
    if (std::string("+-*/^").find(c) != std::string::npos) {
        ++pos_;
        return { TokenType::Operator, std::string(1, c) };
    }

    // неизвестный символ
    throw std::runtime_error(std::string("Unknown character in expression: ") + c);
}
