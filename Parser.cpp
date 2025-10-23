#include "Parser.h"
#include <stack>
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::string& expression)
    : tokenizer_(expression)
{
    next();
}

void Parser::next() {
    current_ = tokenizer_.next();
}

int Parser::precedence(const std::string& op) const {
    // приоритеты операторов
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}

bool Parser::isRightAssociative(const std::string& op) const {
    // только возведение в степень правоассоциативно
    return op == "^";
}

std::vector<RpnItem> Parser::parse() {
    std::vector<RpnItem> output;
    std::stack<Token> ops;

    while (current_.type != TokenType::End) {
        if (current_.type == TokenType::Number) {
            output.push_back({ TokenType::Number, current_.text, current_.value });
            next();
        }
        else if (current_.type == TokenType::Identifier) {
            // функци€ Ч запоминаем как идентификатор
            ops.push(current_);
            next();
        }
        else if (current_.type == TokenType::Operator) {
            std::string op1 = current_.text;
            while (!ops.empty() && ops.top().type == TokenType::Operator) {
                std::string op2 = ops.top().text;
                if ((isRightAssociative(op1) && precedence(op1) < precedence(op2)) ||
                    (!isRightAssociative(op1) && precedence(op1) <= precedence(op2))) {
                    output.push_back({ TokenType::Operator, op2 });
                    ops.pop();
                }
                else break;
            }
            ops.push(current_);
            next();
        }
        else if (current_.type == TokenType::LeftParen) {
            ops.push(current_);
            next();
        }
        else if (current_.type == TokenType::RightParen) {
            while (!ops.empty() && ops.top().type != TokenType::LeftParen) {
                output.push_back({ ops.top().type, ops.top().text });
                ops.pop();
            }
            if (ops.empty())
                throw std::runtime_error("Mismatched parentheses");

            ops.pop(); // remove '('
            // если сверху функци€ Ч добавить еЄ в выход
            if (!ops.empty() && ops.top().type == TokenType::Identifier) {
                output.push_back({ ops.top().type, ops.top().text });
                ops.pop();
            }
            next();
        }
        else {
            throw std::runtime_error("Unexpected token: " + current_.text);
        }
    }

    while (!ops.empty()) {
        if (ops.top().type == TokenType::LeftParen || ops.top().type == TokenType::RightParen)
            throw std::runtime_error("Mismatched parentheses at end");
        output.push_back({ ops.top().type, ops.top().text });
        ops.pop();
    }

    return output;
}
