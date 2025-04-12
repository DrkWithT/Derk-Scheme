#include <utility>
#include <format>
#include <sstream>
#include "syntax/exprs.hpp"

namespace DerkScheme::Syntax {
    Datum::Datum(const Frontend::Token& token, Semantics::BasicTypeTag tag)
    : m_token {token}, m_type {tag} {}

    const Frontend::Token& Datum::token() const noexcept {
        return m_token;
    }

    Semantics::BasicTypeTag Datum::basic_type() const noexcept {
        return m_type;
    }

    Semantics::ListTag Datum::listy_tag() const noexcept {
        return Semantics::ListTag::expr_datum;
    }

    std::size_t Datum::size() const noexcept {
        return 1;
    }

    std::string Datum::to_string() const {
        return std::format("Datum (type = {}, token = (tag = {}, line = {}, col = {}))", static_cast<int>(m_type), static_cast<int>(m_token.tag), m_token.line, m_token.column);
    }

    void Datum::accept_visitor(ExprVisitor<void>& visitor) const {
        visitor.visit_datum(*this);
    }


    Listy::Listy()
    : m_items {} {}

    Listy::Listy(ExprList items) noexcept
    : m_items {std::move(items)}, m_count {0UL} {
        for (auto items_it = m_items.cbegin(), items_end = m_items.cend(); items_it != items_end; items_it++) {
            ++m_count;
        }
    }

    const ExprList& Listy::items() const noexcept {
        return m_items;
    }

    Semantics::BasicTypeTag Listy::basic_type() const noexcept {
        return Semantics::BasicTypeTag::listy;
    }

    Semantics::ListTag Listy::listy_tag() const noexcept {
        return Semantics::ListTag::expr_multiple;
    }

    std::size_t Listy::size() const noexcept {
        return m_count;
    }

    std::string Listy::to_string() const {
        static std::ostringstream sout;

        sout.str("");

        sout << "Listy (type = " << static_cast<int>(basic_type()) << ", items = [";

        for (const auto& p : m_items) {
            sout << p->to_string() << ", ";
        }

        return sout.str();
    }

    void Listy::accept_visitor(ExprVisitor<void>& visitor) const {
        visitor.visit_listy(*this);
    }


    VariableDefine::VariableDefine(std::string name, ExprPtr init_expr) noexcept
    : m_name {std::move(name)}, m_init_expr {std::move(init_expr)} {}

    const std::string& VariableDefine::name() const noexcept {
        return m_name;
    }

    const ExprPtr& VariableDefine::init_expr() const noexcept {
        return m_init_expr;
    }

    Semantics::BasicTypeTag VariableDefine::basic_type() const noexcept {
        return m_init_expr->basic_type();
    }

    Semantics::ListTag VariableDefine::listy_tag() const noexcept {
        return Semantics::ListTag::def_variable;
    }

    std::size_t VariableDefine::size() const noexcept {
        return 1;
    }

    std::string VariableDefine::to_string() const {
        static std::ostringstream sout;
        sout.str("");
        
        sout << "VariableDefine(type = " << static_cast<int>(basic_type()) << ", init_expr = " << m_init_expr->to_string() << ')';

        return sout.str();
    }

    void VariableDefine::accept_visitor(ExprVisitor<void>& visitor) const {
        visitor.visit_variable_define(*this);
    }


    LambdaDefine::LambdaDefine(std::vector<std::string> params, ExprPtr body) noexcept
    : m_params {std::move(params)}, m_body {std::move(body)} {}

    const std::vector<std::string>& LambdaDefine::params() const noexcept {
        return m_params;
    }

    const ExprPtr& LambdaDefine::body() const noexcept {
        return m_body;
    }

    Semantics::BasicTypeTag LambdaDefine::basic_type() const noexcept {
        return Semantics::BasicTypeTag::callable;
    }

    Semantics::ListTag LambdaDefine::listy_tag() const noexcept {
        return Semantics::ListTag::def_lambda;
    }

    std::size_t LambdaDefine::size() const noexcept {
        return 1;
    }

    std::string LambdaDefine::to_string() const {
        return std::format("LambdaDefine(type = {}, params = ..., body = ...)", static_cast<int>(basic_type()));
    }

    void LambdaDefine::accept_visitor(ExprVisitor<void>& visitor) const {
        return visitor.visit_lambda_define(*this);
    }


    Cond::Cond(std::vector<Case> cases) noexcept
    : m_cases {std::move(cases)}, m_count {0} {
        for (auto cases_it = m_cases.cbegin(), cases_end = m_cases.cend(); cases_it != cases_end; cases_it++) {
            ++m_count;
        }
    }

    const std::vector<Case>& Cond::cases() const noexcept {
        return m_cases;
    }

    Semantics::BasicTypeTag Cond::basic_type() const noexcept {
        return Semantics::BasicTypeTag::unknown;
    }

    Semantics::ListTag Cond::listy_tag() const noexcept {
        return Semantics::ListTag::block_do;
    }

    std::size_t Cond::size() const noexcept {
        return m_count;
    }

    std::string Cond::to_string() const {
        return std::format("Cond(count = {}, cases = ...)", m_count);
    }

    void Cond::accept_visitor(ExprVisitor<void>& visitor) const {
        visitor.visit_cond(*this);
    }
}