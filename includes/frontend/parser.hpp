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
        missing_token,
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

        [[nodiscard]] Syntax::AST parse_program();

    private:
        template <ParseErrorGroup ParseErr>
        void report_parse_error(const Token& culprit, std::string_view message) {
            m_errored = true;

            if constexpr (ParseErr == ParseErrorGroup::missing_token) {
                std::print(std::cerr, "Missing a different token at [ln. {}, col. {}]:\nLexeme \"{}\" found instead.\nNote: {}\n", culprit.line, culprit.column, create_token_sv(culprit, m_source_2), message);
            } else if constexpr (ParseErr == ParseErrorGroup::invalid_token) {
                std::print(std::cerr, "Invalid token [ln. {}, col. {}]:\nLexeme \"{}\"\nNote: {}\n", culprit.line, culprit.column, create_token_sv(culprit, m_source_2), message);
            } else {
                std::print(std::cerr, "Unexpected token [ln. {}, col. {}]:\nLexeme \"{}\"\nNote: {}\n", culprit.line, culprit.column, create_token_sv(culprit, m_source_2), message);
            }
        }

        const Token& previous() const noexcept;
        const Token& current() const noexcept;
        [[nodiscard]] bool atEOF() const noexcept;

        [[nodiscard]] constexpr bool match() const noexcept {
            return true;
        }

        template <ParserPeekOpt PeekOpt, typename Tag, typename... TagRest> requires (std::is_same_v<std::remove_reference_t<Tag>, TokenTag>)
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
        void recover_parse() noexcept;
        void consume() noexcept;

        template <ParserPeekOpt PeekOpt, typename Tag, typename... TagRest> requires (std::is_same_v<std::remove_reference_t<Tag>, TokenTag>)
        [[maybe_unused]] bool consume(Tag&& tag, TagRest&&... rest) noexcept {
            if (atEOF()) {
                return true;
            }

            if constexpr (PeekOpt == ParserPeekOpt::current) {
                if (match<ParserPeekOpt::current>(tag, rest...)) {
                    consume();
                    return true;
                }
            } else if constexpr (PeekOpt == ParserPeekOpt::previous) {
                if (match<ParserPeekOpt::previous>(tag, rest...)) {
                    consume();
                    return true;
                }
            }

            recover_parse();
            return false;
        }

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