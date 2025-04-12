#pragma once

#include <forward_list>
#include <memory>
#include "frontend/token.hpp"
#include "semantics/tags.hpp"
#include "syntax/expr_base.hpp"

namespace DerkScheme::Syntax {
    /**
     * @brief Provides an alias for a base expression pointer.
     */
    using ExprPtr = std::unique_ptr<Expr>;

    /**
     * @brief Provides an alias for a list of any expression.
     */
    using ExprList = std::forward_list<ExprPtr>;

    class Datum : public Expr {
    public:
        Datum() = delete;
        Datum(const Frontend::Token& token, Semantics::BasicTypeTag tag);

        const Frontend::Token& token() const noexcept;

        [[nodiscard]] Semantics::BasicTypeTag basic_type() const noexcept override = 0;
        [[nodiscard]] Semantics::ListTag listy_tag() const noexcept override = 0;
        [[nodiscard]] std::size_t size() const noexcept override = 0;
        [[nodiscard]] std::string to_string() const override = 0;

        void accept_visitor(ExprVisitor<void>&) const override = 0;
    private:
        Frontend::Token m_token;
        Semantics::BasicTypeTag m_type;
    };

    class Listy : public Expr {
    public:
        Listy();
        Listy(ExprList items) noexcept;

        const ExprList& items() const noexcept;

        [[nodiscard]] Semantics::BasicTypeTag basic_type() const noexcept override = 0;
        [[nodiscard]] Semantics::ListTag listy_tag() const noexcept override = 0;
        [[nodiscard]] std::size_t size() const noexcept override = 0;
        [[nodiscard]] std::string to_string() const override = 0;

        void accept_visitor(ExprVisitor<void>&) const override = 0;
    private:
        ExprList m_items;
        std::size_t m_count;
    };

    class VariableDefine : public Expr {
    public:
        VariableDefine() = delete;
        VariableDefine(std::string name, ExprPtr init_expr) noexcept;

        const std::string& name() const noexcept;
        const ExprPtr& init_expr() const noexcept;

        [[nodiscard]] Semantics::BasicTypeTag basic_type() const noexcept override = 0;
        [[nodiscard]] Semantics::ListTag listy_tag() const noexcept override = 0;
        [[nodiscard]] std::size_t size() const noexcept override = 0;
        [[nodiscard]] std::string to_string() const override = 0;

        void accept_visitor(ExprVisitor<void>&) const override = 0;
    private:
        std::string m_name;
        ExprPtr m_init_expr;
    };

    class LambdaDefine : public Expr {
    public:
        LambdaDefine() = delete;
        LambdaDefine(ExprList params, ExprPtr body) noexcept;

        const ExprList& params() const noexcept;
        const ExprPtr& body() const noexcept;

        [[nodiscard]] Semantics::BasicTypeTag basic_type() const noexcept override = 0;
        [[nodiscard]] Semantics::ListTag listy_tag() const noexcept override = 0;
        [[nodiscard]] std::size_t size() const noexcept override = 0;
        [[nodiscard]] std::string to_string() const override = 0;

        void accept_visitor(ExprVisitor<void>&) const override = 0;
    private:
        ExprList m_params;
        ExprPtr m_body;
    };

    struct Case {
        ExprPtr test;
        ExprPtr result;
    };

    class Cond : public Expr {
    public:
        Cond() = delete;
        Cond(std::forward_list<Case> cases) noexcept;

        const std::forward_list<Case>& cases() const noexcept;

        [[nodiscard]] Semantics::BasicTypeTag basic_type() const noexcept override = 0;
        [[nodiscard]] Semantics::ListTag listy_tag() const noexcept override = 0;
        [[nodiscard]] std::size_t size() const noexcept override = 0;
        [[nodiscard]] std::string to_string() const override = 0;

        void accept_visitor(ExprVisitor<void>&) const override = 0;
    private:
        std::forward_list<Case> m_cases;
        int m_count;
    };
}