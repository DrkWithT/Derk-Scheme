#include "frontend/lexer.hpp"
#include <initializer_list>

namespace DerkScheme::Frontend {
    static const std::initializer_list<LexicalEntry> entries = {
        LexicalEntry {"if", WordRole::special_form},
        {"cond", WordRole::special_form},
        {"case", WordRole::special_form},
        {"define", WordRole::special_form},
        {"let", WordRole::special_form},
        {"do", WordRole::special_form}
    };

    Lexer::Lexer(std::string_view source)
    : m_special_forms {}, m_srcv {source}, m_pos {0}, m_end {static_cast<int>(source.size())}, m_line {1}, m_column {1} {
        for (const auto& [text, role] : entries) {
            if (role == WordRole::special_form) {
                m_special_forms.insert(text);
            }
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

    bool Lexer::atEnd() const noexcept {
        return m_pos >= m_end;
    }

    Token Lexer::lexSingle(TokenTag tag) noexcept {
        const auto token_begin = m_pos;

        update_source_location(m_srcv[m_pos]);
        ++m_pos;

        return {
            .tag = tag,
            .begin = token_begin,
            .length = 1,
            .line = m_line,
            .column = m_column
        };
    }

    // Token Lexer::lexSpacing() noexcept; // todo

    // Token Lexer::lexWord() noexcept;

    // Token Lexer::lexBoolean() noexcept;

    // Token Lexer::lexNumber() noexcept;

    // Token Lexer::lexString() noexcept;
}