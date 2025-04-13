#pragma once

#include <string>
#include <variant>
#include <vector>
#include "semantics/tags.hpp"
#include "syntax/exprs.hpp"
#include "meta/constraints.hpp"

namespace DerkScheme::Evaluator {
    class Value;

    class Null {
    public:
        Null() noexcept = default;

        [[nodiscard]] Semantics::BasicTypeTag type() const noexcept;
        [[nodiscard]] bool as_truthy() const noexcept;
        [[nodiscard]] bool is_constant() const noexcept;
    };

    class Boolean {
    public:
        explicit Boolean(bool b) noexcept;

        [[nodiscard]] bool value() const noexcept;
        void value(bool b) noexcept;

        [[nodiscard]] Semantics::BasicTypeTag type() const noexcept;
        [[nodiscard]] bool as_truthy() const noexcept;
        [[nodiscard]] bool is_constant() const noexcept;

    private:
        bool m_data;
    };

    class ExactNumber {
    public:
        explicit ExactNumber(int i) noexcept;

        [[nodiscard]] int value() const noexcept;
        void value(int i) noexcept;

        [[nodiscard]] Semantics::BasicTypeTag type() const noexcept;
        [[nodiscard]] bool as_truthy() const noexcept;
        [[nodiscard]] bool is_constant() const noexcept;
    
    private:
        int m_data;
    };

    class RealNumber {
    public:
        explicit RealNumber(float f) noexcept;

        [[nodiscard]] float value() const noexcept;
        void value(float f) noexcept;

        [[nodiscard]] Semantics::BasicTypeTag type() const noexcept;
        [[nodiscard]] bool as_truthy() const noexcept;
        [[nodiscard]] bool is_constant() const noexcept;
    private:
        float m_data;
    };

    class String {
    public:
        String(const std::string& s);

        [[nodiscard]] const std::string& value() const noexcept;
        void value(const std::string& s);

        [[nodiscard]] Semantics::BasicTypeTag type() const noexcept;
        [[nodiscard]] bool as_truthy() const noexcept;
        [[nodiscard]] bool is_constant() const noexcept;

    private:
        std::string m_data;
    };

    class Lambda {
    public:
        Lambda(std::vector<std::string> params, Syntax::ExprPtr body) noexcept;

        const std::vector<std::string>& params() const noexcept;
        const Syntax::ExprPtr& body() const noexcept;

        [[nodiscard]] Semantics::BasicTypeTag type() const noexcept;
        [[nodiscard]] bool as_truthy() const noexcept;
        [[nodiscard]] bool is_constant() const noexcept;

    private:
        std::vector<std::string> m_params;
        Syntax::ExprPtr m_body;
    };

    class Value {
    public:
        template <typename SchemeValue> requires (Meta::ValueKind<SchemeValue>)
        Value(SchemeValue&& value)
        : m_data {value} {}

        std::variant<Null, Boolean, ExactNumber, RealNumber, String, Lambda>& data() noexcept;
        [[nodiscard]] Semantics::BasicTypeTag type() const noexcept;
        explicit operator bool() const noexcept;
        [[nodiscard]] bool is_constant() const noexcept;

    private:
        std::variant<Null, Boolean, ExactNumber, RealNumber, String, Lambda> m_data;
    };
}