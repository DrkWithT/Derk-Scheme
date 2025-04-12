/**
 * @file test_lexer.cpp
 * @author DrkWithT
 * @brief Unit test for lexer (must recognize all valid tokens!)
 * @version 0.1.0
 * @date 2025-04-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <print>
#include <string_view>
#include "frontend/token.hpp"
#include "frontend/lexer.hpp"

using namespace DerkScheme;

constexpr std::string_view test_1_source_copy = "(define x 42)\n"
"(define y 24)\n"
"(define avg-pair\n"
    "\t(lambda (a b)\n"
        "\t\t(/ (+ a b) 2)))\n"
"(define answer (avg-pair x y))\n"
"(display answer)\n";

[[nodiscard]] bool do_basic_lexer_test(std::string_view sample) noexcept {
    Frontend::Lexer lexer {sample};
    Frontend::Token temp;

    while ((temp = lexer.next()).tag != Frontend::TokenTag::eof) {
        if (temp.tag == Frontend::TokenTag::unknown) {
            std::print(std::cerr, "Lexical Error for culprit:\n{}\n", Frontend::create_token_sv(temp, sample));
            return false;
        }
    }

    return true;
}

int main() {
    if (not do_basic_lexer_test(test_1_source_copy)) {
        return 1;
    }
}
