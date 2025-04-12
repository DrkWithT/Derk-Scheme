#include <utility>
#include "frontend/parser.hpp"

namespace DerkScheme::Frontend {
    Parser::Parser(std::string_view source) noexcept
    : m_lexer {source}, m_previous {}, m_current {}, m_source_2 {source}, m_errored {false} {
        m_previous.tag = TokenTag::unknown;
        m_current.tag = TokenTag::unknown;
    }

    const Token& Parser::previous() const noexcept {
        return m_previous;
    }

    const Token& Parser::current() const noexcept {
        return m_current;
    }

    bool Parser::atEOF() const noexcept {
        return m_current.tag == TokenTag::eof;
    }

    Token Parser::advance() noexcept {
        Token temp;

        while (true) {
            temp = m_lexer.next();
            const auto temp_tag = temp.tag;

            if (temp_tag == TokenTag::spacing) {
                continue;
            }

            if (temp_tag == TokenTag::unknown) {
                m_errored = true;
                continue;
            }

            break;
        }

        return temp;
    }


    Syntax::ExprPtr parse_program();

    Syntax::ExprPtr parse_top();

    Syntax::ExprPtr parse_variable();

    /// NOTE: params parsing logic inside!
    Syntax::ExprPtr parse_lambda();

    Syntax::ExprPtr parse_nestable();

    Syntax::ExprPtr parse_do();

    /// NOTE: case parsing logic inside!
    Syntax::ExprPtr parse_cond();

    Syntax::ExprPtr parse_compute();

    Syntax::ExprPtr parse_datum();
}