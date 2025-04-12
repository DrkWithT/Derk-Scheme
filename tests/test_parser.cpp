/**
 * @file test_parser.cpp
 * @author DrkWithT
 * @brief Implements parser unit test with sample code 1.
 * @version 0.1.0
 * @date 2025-04-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <print>
#include <string_view>
#include "syntax/exprs.hpp"
#include "frontend/parser.hpp"

using namespace DerkScheme;

constexpr std::string_view test_1_source_copy = "(define x 42)\n"
"(define y 24)\n"
"(define (my-average a b)\n"
    "(+ (/ a 2)\n"
        "(/ b 2)))\n"
"(define answer (my-average x y))\n"
"(display answer)\n";

[[nodiscard]] bool test_sample_parse(std::string_view source) noexcept {
    Frontend::Parser parser {source};

    auto [tops, ok] = parser.parse_program();

    if (not ok) {
        return false;
    }

    std::print("Top-level constructs parsed:\n\n");
    for (const auto& top_expr : tops) {
        std::print("{}\n", top_expr->to_string());
    }

    return true;
}

int main() {
    if (not test_sample_parse(test_1_source_copy)) {
        return 1;
    }
}