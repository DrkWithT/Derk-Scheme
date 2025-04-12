#pragma once

namespace DerkScheme::Syntax {
    class Datum;
    class Listy;
    class VariableDefine;
    class LambdaDefine;
    class Cond;

    template <typename Result>
    class ExprVisitor {
    public:
        virtual ~ExprVisitor() noexcept = default;
        virtual Result visit_datum(const Datum&) = 0;
        virtual Result visit_listy(const Listy&) = 0;
        virtual Result visit_variable_define(const VariableDefine&) = 0;
        virtual Result visit_lambda_define(const LambdaDefine&) = 0;
        virtual Result visit_cond(const Cond&) = 0;
    };
}