#pragma once

#include <set>
#include <string_view>
#include "frontend/token.hpp"

namespace DerkScheme::Frontend {
    enum class WordRole {
        special_form,
        none
    };

    struct LexicalEntry {
        std::string_view text;
        WordRole role;
    };

    namespace LexicalMatchers {
        [[nodiscard]] constexpr bool match_spacing(char c) noexcept {
            return c == ' ' or c == '\t' or c == '\n' or c == '\r';
        }

        [[nodiscard]] constexpr bool match_alphabetic(char c) noexcept {
            return (c >= 'A' and c <= 'Z') or (c >= 'a' and c <= 'z') or c == '_';
        }

        [[nodiscard]] constexpr bool match_digit(char c) noexcept {
            return c >= '0' and c <= '9';
        }

        [[nodiscard]] constexpr bool match_op_symbol(char c) noexcept {
            return c == '+' or c == '-' or c == '*' or c == '/' or c == '%';
        }

        [[nodiscard]] constexpr bool match_value_mark(char c) noexcept {
            return c == '#' or c == '\"';
        }
    };

    class Lexer {
    public:
        Lexer() = delete;
        Lexer(std::string_view source);

        [[nodiscard]] Token next() noexcept;

    private:
        [[nodiscard]] bool atEnd() const noexcept;

        void update_source_location(char c) noexcept;

        [[nodiscard]] Token lexSingle(TokenTag tag) noexcept;
        [[nodiscard]] Token lexSpacing() noexcept;
        [[nodiscard]] Token lexWord() noexcept;
        [[nodiscard]] Token lexBoolean() noexcept;
        [[nodiscard]] Token lexNumber() noexcept;
        [[nodiscard]] Token lexString() noexcept;

        std::set<std::string_view> m_special_forms;
        std::string_view m_srcv;
        int m_pos;
        int m_end;
        int m_line;
        int m_column;
    };
}