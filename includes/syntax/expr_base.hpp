#pragma once

#include <string>
#include "syntax/expr_visitor.hpp"
#include "semantics/tags.hpp"

namespace DerkScheme::Syntax {
    class Expr {
    public:
        virtual ~Expr() noexcept = default;
        virtual Semantics::BasicTypeTag basic_type() const noexcept = 0;
        virtual Semantics::ListTag listy_tag() const noexcept = 0;
        virtual std::size_t size() const noexcept = 0;
        virtual std::string to_string() const = 0;

        virtual void accept_visitor(ExprVisitor<void>&) const = 0;
    };
}