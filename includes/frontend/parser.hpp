#pragma once

#include <iostream>
#include <print>
#include <string_view>
#include <type_traits>
#include "frontend/token.hpp"
#include "frontend/lexer.hpp"
#include "syntax/exprs.hpp"

namespace DerkScheme::Frontend {
    enum class ParserPeekOpt {
        previous,
        current
    };

    enum class ParseErrorGroup {
        invalid_token,
        unexpected_token
    };

    class Parser {
    private:
        Lexer m_lexer;
        Token m_previous;
        Token m_current;
        std::string_view m_source_2;
        bool m_errored;

    public:
        Parser() = delete;
        Parser(std::string_view source) noexcept;

    private:
        template <ParseErrorGroup ParseErr>
        void report_parse_error(const Token& culprit, std::string_view message) {
            if constexpr (ParseErr == ParseErrorGroup::invalid_token) {
                std::print(std::cerr, "Invalid token [ln. {}, col. {}]:\nLexeme \"{}\" is unknown in Scheme!\n", culprit.line, culprit.column, create_token_sv(culprit, m_source_2));
            } else {
                std::print(std::cerr, "Unexpected token [ln. {}, col. {}]:\nLexeme \"{}\"\nNote: {}", culprit.line, culprit.column, create_token_sv(culprit, m_source_2), message);
            }
        }

        const Token& previous() const noexcept;
        const Token& current() const noexcept;
        [[nodiscard]] bool atEOF() const noexcept;

        template <ParserPeekOpt PeekOpt, typename Tag, typename... TagRest> requires (std::is_same_v<Tag, TokenTag>)
        [[nodiscard]] bool match(Tag&& tag, TagRest&&... rest) const noexcept {
            if constexpr (PeekOpt == ParserPeekOpt::current) {
                const auto curr_tag = m_current.tag;

                return ((curr_tag == tag) or ... or (curr_tag == rest));
            } else {
                const auto prev_tag = m_previous.tag;
                
                return ((prev_tag == tag) or ... or (prev_tag == rest));
            }
        }

        [[nodiscard]] Token advance() noexcept;

        [[nodiscard]] Syntax::ExprPtr parse_program();
        [[nodiscard]] Syntax::ExprPtr parse_top();
        [[nodiscard]] Syntax::ExprPtr parse_variable();
        /// NOTE: params parsing logic inside!
        [[nodiscard]] Syntax::ExprPtr parse_lambda();
        [[nodiscard]] Syntax::ExprPtr parse_nestable();
        [[nodiscard]] Syntax::ExprPtr parse_do();
        /// NOTE: case parsing logic inside!
        [[nodiscard]] Syntax::ExprPtr parse_cond();
        [[nodiscard]] Syntax::ExprPtr parse_compute();
        [[nodiscard]] Syntax::ExprPtr parse_datum();
    };
}