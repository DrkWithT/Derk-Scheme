#include <utility>
#include "frontend/parser.hpp"

namespace DerkScheme::Frontend {
    Parser::Parser(std::string_view source) noexcept
    : m_lexer {source}, m_previous {}, m_current {}, m_source_2 {source}, m_errored {false} {
        m_previous.tag = TokenTag::unknown;
        m_current.tag = TokenTag::unknown;
        consume();
    }

    Syntax::AST Parser::parse_program() {
        std::vector<Syntax::ExprPtr> tops;

        while (not atEOF()) {
            tops.emplace_back(parse_top());
        }

        return {
            .items = std::move(tops),
            .ok = not m_errored
        };
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

            break;
        }

        return temp;
    }

    void Parser::recover_parse() noexcept {
        while (not atEOF() and not match<ParserPeekOpt::current>(TokenTag::right_paren)) {
            consume();
        }

        consume();
    }

    void Parser::consume() noexcept {
        m_previous = m_current;
        m_current = advance();
    }

    Syntax::ExprPtr Parser::parse_top() {
        if (not consume<ParserPeekOpt::current>(TokenTag::left_paren)) {
            report_parse_error<ParseErrorGroup::missing_token>(current(), "Expected '('.");
            return nullptr;
        }

        const auto current_lexeme = create_token_sv(current(), m_source_2);
        Syntax::ExprPtr temp {};

        if (current_lexeme == "define" or current_lexeme == "let!") {
            temp = parse_variable();
        } else if (current_lexeme == "lambda") {
            temp = parse_lambda();
        } else {
            temp = parse_compute();
        }

        consume<ParserPeekOpt::current>(TokenTag::right_paren);

        return temp;
    }

    Syntax::ExprPtr Parser::parse_variable() {
        consume();
        consume<ParserPeekOpt::current>(TokenTag::identifier_name);

        auto var_name = create_token_info_str(previous(), m_source_2);
        auto var_init_expr = parse_nestable();

        return std::make_unique<Syntax::VariableDefine>(std::move(var_name), std::move(var_init_expr));
    }

    /// NOTE: params parsing logic inside!
    Syntax::ExprPtr Parser::parse_lambda() {
        consume();
        consume<ParserPeekOpt::current>(TokenTag::left_paren);

        std::vector<std::string> param_vec;
        bool closed_paren = false;

        while (not atEOF()) {
            if (match<ParserPeekOpt::current>(TokenTag::right_paren)) {
                consume();
                closed_paren = true;
                break;
            }

            consume<ParserPeekOpt::current>(TokenTag::identifier_name);
            param_vec.emplace_back(create_token_info_str(previous(), m_source_2));
        }

        if (not closed_paren) {
            report_parse_error<ParseErrorGroup::missing_token>(current(), "Expected closing ')' after lambda parameters.");
            return nullptr;
        }

        auto body = parse_nestable();

        return std::make_unique<Syntax::LambdaDefine>(std::move(param_vec), std::move(body));
    }

    Syntax::ExprPtr Parser::parse_nestable() {
        auto enclosed = false;

        if (match<ParserPeekOpt::current>(TokenTag::left_paren)) {
            consume();
            enclosed = true;
        }

        const auto current_lexeme = create_token_sv(current(), m_source_2);
        Syntax::ExprPtr temp {};

        if (current_lexeme == "lambda") {
            temp = parse_lambda();
        } else if (current_lexeme == "do") {
            temp = parse_do();
        } else if (current_lexeme == "cond") {
            temp = parse_cond();
        } else {
            temp = parse_compute();
        }

        if (enclosed) {
            consume<ParserPeekOpt::current>(TokenTag::right_paren);   
        }

        return temp;
    }

    Syntax::ExprPtr Parser::parse_do() {
        consume();

        Syntax::ExprList steps;
        bool closed_paren = false;

        while (not atEOF()) {
            if (match<ParserPeekOpt::current>(TokenTag::right_paren)) {
                closed_paren = true;
                break;
            }

            steps.emplace_back(parse_top());
        }

        if (not closed_paren) {
            report_parse_error<ParseErrorGroup::missing_token>(current(), "Expected closing ')' in 'do' construct.");
            return nullptr;
        }

        return std::make_unique<Syntax::Listy>(std::move(steps));
    }

    /// NOTE: case parsing logic inside!
    Syntax::ExprPtr Parser::parse_cond() {
        consume();

        std::vector<Syntax::Case> cases;
        bool closed_paren = false;

        while (atEOF()) {
            if (match<ParserPeekOpt::current>(TokenTag::right_paren)) {
                closed_paren = true;
                break;
            }

            consume<ParserPeekOpt::current>(TokenTag::left_paren);

            cases.emplace_back(Syntax::Case {
                .test = parse_compute(),
                .result = parse_compute()
            });

            consume<ParserPeekOpt::current>(TokenTag::right_paren);
        }

        if (not closed_paren) {
            report_parse_error<ParseErrorGroup::missing_token>(current(), "Expected closing ')'.");
            return nullptr;
        }

        return std::make_unique<Syntax::Cond>(std::move(cases));
    }

    Syntax::ExprPtr Parser::parse_compute() {
        Syntax::ExprList items;
        bool closed_paren = false;

        while (not atEOF()) {
            if (match<ParserPeekOpt::current>(TokenTag::right_paren)) {
                closed_paren = true;
                break;
            }

            if (match<ParserPeekOpt::current>(TokenTag::left_paren)) {
                consume();
                items.emplace_back(parse_compute());
                consume<ParserPeekOpt::current>(TokenTag::right_paren);
            } else {
                items.emplace_back(parse_datum());
            }
        }

        if (not closed_paren) {
            report_parse_error<ParseErrorGroup::missing_token>(current(), "Expected closing ')' for valued expression.");
            return nullptr;
        }

        return std::make_unique<Syntax::Listy>(std::move(items));
    }

    Syntax::ExprPtr Parser::parse_datum() {
        auto temp = current();
        const auto token_tag = temp.tag;

        switch (token_tag) {
        case TokenTag::identifier_name:
            consume();
            return std::make_unique<Syntax::Datum>(temp, Semantics::BasicTypeTag::unknown);
        case TokenTag::literal_boolean_false:
        case TokenTag::literal_boolean_true:
            consume();
            return std::make_unique<Syntax::Datum>(temp, Semantics::BasicTypeTag::boolean);
        case TokenTag::literal_number_exact:
            consume();
            return std::make_unique<Syntax::Datum>(temp, Semantics::BasicTypeTag::number_exact);
        case TokenTag::literal_number_real:
            consume();    
            return std::make_unique<Syntax::Datum>(temp, Semantics::BasicTypeTag::number_real);
        case TokenTag::literal_string:
            consume();
            return std::make_unique<Syntax::Datum>(temp, Semantics::BasicTypeTag::string);
        default:
            report_parse_error<ParseErrorGroup::invalid_token>(temp, "Invalid token for datum, as only names, boolean, numeric, or string literals are allowed.");
            recover_parse();
            break;
        }

        return nullptr;
    }
}