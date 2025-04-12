#include "frontend/lexer.hpp"
#include "frontend/token.hpp"
#include <initializer_list>

namespace DerkScheme::Frontend {
    static const std::initializer_list<LexicalEntry> entries = {
        LexicalEntry {"if", WordRole::special_form},
        {"cond", WordRole::special_form},
        {"case", WordRole::special_form},
        {"define", WordRole::special_form},
        {"let!", WordRole::special_form},
        {"do", WordRole::special_form},
        {"lambda", WordRole::special_form}
    };

    Lexer::Lexer(std::string_view source)
    : m_special_forms {}, m_srcv {source}, m_pos {0}, m_end {static_cast<int>(source.size())}, m_line {1}, m_column {1} {
        for (const auto& [text, role] : entries) {
            if (role == WordRole::special_form) {
                m_special_forms.insert(text);
            }
        }
    }

    bool Lexer::atEnd() const noexcept {
        return m_pos >= m_end;
    }

    void Lexer::update_source_location(char c) noexcept {
        if (c == '\n') {
            ++m_line;
            m_column = 1;
        } else {
            ++m_column;
        }
    }
    
    Token Lexer::next() noexcept {
        if (atEnd()) {
            return {
                .tag = TokenTag::eof,
                .begin = m_pos,
                .length = 1,
                .line = -1,
                .column = -1
            };
        }

        const auto peeked = m_srcv[m_pos];

        switch (peeked) {
            case '(':
                return lexSingle(TokenTag::left_paren);
            case ')':
                return lexSingle(TokenTag::right_paren);
            case '[':
                return lexSingle(TokenTag::left_bracket);
            case ']':
                return lexSingle(TokenTag::right_bracket);
            case '\"':
                return lexString();
            case '#':
                return lexBoolean();
            default:
                break;
        }

        if (LexicalMatchers::match_spacing(peeked)) {
            return lexSpacing();
        } else if (LexicalMatchers::match_alphabetic(peeked) or LexicalMatchers::match_op_symbol(peeked)) {
            return lexWord();
        } else if (LexicalMatchers::match_digit(peeked)) {
            return lexNumber();
        }

        return lexSingle(TokenTag::unknown);
    }

    Token Lexer::lexSingle(TokenTag tag) noexcept {
        const auto token_begin = m_pos;
        const auto token_col_1 = m_column;

        update_source_location(m_srcv[m_pos]);
        ++m_pos;

        return {
            .tag = tag,
            .begin = token_begin,
            .length = 1,
            .line = m_line,
            .column = token_col_1
        };
    }

    Token Lexer::lexSpacing() noexcept {
        auto token_begin = m_pos;
        auto token_length = 0;
        const auto token_line_1 = m_line;
        const auto token_col_1 = m_column;

        while (not atEnd()) {
            const auto temp = m_srcv[m_pos];

            if (not LexicalMatchers::match_spacing(temp)) {
                break;
            }

            update_source_location(temp);

            ++token_length;
            ++m_pos;
        }

        return {
            .tag = TokenTag::spacing,
            .begin = token_begin,
            .length = token_length,
            .line = token_line_1,
            .column = token_col_1
        };
    }

    Token Lexer::lexWord() noexcept {
        auto token_begin = m_pos;
        auto token_length = 0;
        const auto token_line_1 = m_line;
        const auto token_col_1 = m_column;

        while (not atEnd()) {
            const auto temp = m_srcv[m_pos];

            if (not LexicalMatchers::match_alphabetic(temp) and not LexicalMatchers::match_digit(temp) and not LexicalMatchers::match_op_symbol(temp)) {
                break;
            }

            update_source_location(temp);
            ++token_length;
            ++m_pos;
        }

        auto lexeme = m_srcv.substr(token_begin, token_length);

        if (m_special_forms.contains(lexeme)) {
            return {
                .tag = TokenTag::special_name,
                .begin = token_begin,
                .length = token_length,
                .line = token_line_1,
                .column = token_col_1
            };
        }

        return {
            .tag = TokenTag::identifier_name,
            .begin = token_begin,
            .length = token_length,
            .line = token_line_1,
            .column = token_col_1
        };
    }

    Token Lexer::lexBoolean() noexcept {
        update_source_location(m_srcv[m_pos]);
        ++m_pos;

        const auto token_begin = m_pos;
        bool boolean_kind = false;
        const auto token_line_1 = m_line;
        const auto token_col_1 = m_column;

        update_source_location(m_srcv[m_pos]);

        switch (m_srcv[m_pos]) {
            case 't':
                boolean_kind = true;
                break;
            case 'f':
                break;
            default:
                ++m_pos;
                return {
                    .tag = TokenTag::unknown,
                    .begin = token_begin,
                    .length = 2,
                    .line = token_line_1,
                    .column = token_col_1
                };
        }

        ++m_pos;

        return {
            .tag = (boolean_kind) ? TokenTag::literal_boolean_true : TokenTag::literal_boolean_false,
            .begin = token_begin,
            .length = 2,
            .line = token_line_1,
            .column = token_col_1
        };
    }

    Token Lexer::lexNumber() noexcept {
        auto token_begin = m_pos;
        auto token_length = 0;
        auto dots = 0;
        const auto token_line_1 = m_line;
        const auto token_col_1 = m_column;

        while (not atEnd()) {
            const auto temp = m_srcv[m_pos];

            if (not LexicalMatchers::match_digit(temp) and temp != '.') {
                break;
            }

            if (temp == '.') {   
                ++dots;
            }

            update_source_location(temp);

            ++token_length;
            ++m_pos;
        }

        auto temp_tag = TokenTag::unknown;

        switch (dots) {
            case 0:
                temp_tag = TokenTag::literal_number_exact;
                break;
            case 1:
                temp_tag = TokenTag::literal_number_real;
                break;
            default:
                break;
        }

        return {
            .tag = temp_tag,
            .begin = token_begin,
            .length = token_length,
            .line = token_line_1,
            .column = token_col_1
        };
    }

    Token Lexer::lexString() noexcept {
        ++m_pos; // skip leading double quote...
        update_source_location(m_srcv[m_pos]);

        auto token_begin = m_pos;
        auto token_length = 0;
        const auto token_line_1 = m_line;
        const auto token_col_1 = m_column;
        bool closed = false;

        while (not atEnd()) {
            const auto temp = m_srcv[m_pos];

            if (temp == '\"') {
                ++m_pos;
                break;
            }

            update_source_location(temp);

            ++token_length;
            ++m_pos;
        }

        return {
            .tag = (closed) ? TokenTag::literal_string : TokenTag::unknown,
            .begin = token_begin,
            .length = token_length,
            .line = token_line_1,
            .column = token_col_1
        };
    }
}