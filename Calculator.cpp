#include "Tokenizer.h"
#include <iostream>

int main() {
    Tokenizer t("16 + 4 * (3 - 1)");
    Token tok;
    while ((tok = t.next()).type != TokenType::End) {
        std::cout << "Token: [" << tok.text << "]\n";
    }
}
