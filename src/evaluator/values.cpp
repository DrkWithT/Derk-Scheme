#include <utility>
#include <type_traits>
#include "semantics/tags.hpp"
#include "evaluator/values.hpp"

namespace DerkScheme::Evaluator {
    constexpr auto falsy_int = 0;
    constexpr auto falsy_float = 0.0f;

    Semantics::BasicTypeTag Null::type() const noexcept {
        return Semantics::BasicTypeTag::null;
    }

    bool Null::as_truthy() const noexcept {
        return false;
    }

    bool Null::is_constant() const noexcept {
        return true;
    }


    Semantics::BasicTypeTag Boolean::type() const noexcept {
        return Semantics::BasicTypeTag::boolean;
    }

    bool Boolean::as_truthy() const noexcept {
        return m_data;
    }

    bool Boolean::is_constant() const noexcept {
        return true;
    }


    ExactNumber::ExactNumber(int i) noexcept
    : m_data {i} {}

    int ExactNumber::value() const noexcept {
        return m_data;
    }

    void ExactNumber::value(int i) noexcept {
        m_data = i;
    }

    Semantics::BasicTypeTag ExactNumber::type() const noexcept {
        return Semantics::BasicTypeTag::number_exact;
    }

    bool ExactNumber::as_truthy() const noexcept {
        return m_data != falsy_int;
    }

    bool ExactNumber::is_constant() const noexcept {
        return false;
    }


    RealNumber::RealNumber(float f) noexcept
    : m_data {f} {}

    float RealNumber::value() const noexcept {
        return m_data;
    }

    void RealNumber::value(float f) noexcept {
        m_data = f;
    }

    Semantics::BasicTypeTag RealNumber::type() const noexcept {
        return Semantics::BasicTypeTag::number_real;
    }

    bool RealNumber::as_truthy() const noexcept {
        return m_data != falsy_float;
    }

    bool RealNumber::is_constant() const noexcept {
        return false;
    }


    String::String(const std::string& s)
    : m_data {s} {}

    const std::string& String::value() const noexcept {
        return m_data;
    }

    void String::value(const std::string& s) {
        m_data = s;
    }

    Semantics::BasicTypeTag String::type() const noexcept {
        return Semantics::BasicTypeTag::string;
    }

    bool String::as_truthy() const noexcept {
        return not m_data.empty();
    }

    bool String::is_constant() const noexcept {
        return false;
    }


    Lambda::Lambda(std::vector<std::string> params, Syntax::ExprPtr body) noexcept
    : m_params {std::move(params)}, m_body {std::move(body)} {}

    const std::vector<std::string>& Lambda::params() const noexcept {
        return m_params;
    }

    const Syntax::ExprPtr& Lambda::body() const noexcept {
        return m_body;
    }

    Semantics::BasicTypeTag Lambda::type() const noexcept {
        return Semantics::BasicTypeTag::callable;
    }

    bool Lambda::as_truthy() const noexcept {
        return true;
    }

    bool Lambda::is_constant() const noexcept {
        return true;
    }


    std::variant<Null, Boolean, ExactNumber, RealNumber, String, Lambda>& Value::data() noexcept {
        return m_data;
    }

    Semantics::BasicTypeTag Value::type() const noexcept {
        const auto tag = m_data.index();

        switch (tag) {
            case 0:
                return Semantics::BasicTypeTag::null;
            case 1:
                return Semantics::BasicTypeTag::boolean;
            case 2:
                return Semantics::BasicTypeTag::number_exact;
            case 3:
                return Semantics::BasicTypeTag::number_real;
            case 4:
                return Semantics::BasicTypeTag::string;
            case 5:
                return Semantics::BasicTypeTag::callable;
            default:
                break;
        }

        return Semantics::BasicTypeTag::unknown;
    }

    Value::operator bool() const noexcept {
        bool result = false;

        std::visit([&result] <typename CaseT> (CaseT&& arg) {
            using BareT = std::remove_reference_t<CaseT>;

            if constexpr (std::is_same_v<BareT, Null>) {
                result = arg.as_truthy();
            } else if constexpr (std::is_same_v<BareT, Boolean>) {
                result = arg.as_truthy();
            } else if constexpr (std::is_same_v<BareT, ExactNumber>) {
                result = arg.as_truthy();
            } else if constexpr (std::is_same_v<BareT, RealNumber>) {
                result = arg.as_truthy();
            } else if constexpr (std::is_same_v<BareT, String>) {
                result = arg.as_truthy();
            } else if constexpr (std::is_same_v<BareT, Lambda>) {
                result = arg.as_truthy();
            }

            result = false;
        }, m_data);

        return result;
    }

    bool Value::is_constant() const noexcept {
        bool result = false;

        std::visit([&result] <typename CaseT> (CaseT&& arg) {
            using BareT = std::remove_reference_t<CaseT>;

            if constexpr (std::is_same_v<BareT, Null>) {
                result = arg.is_constant();
            } else if constexpr (std::is_same_v<BareT, Boolean>) {
                result = arg.is_constant();
            } else if constexpr (std::is_same_v<BareT, ExactNumber>) {
                result = arg.is_constant();
            } else if constexpr (std::is_same_v<BareT, RealNumber>) {
                result = arg.is_constant();
            } else if constexpr (std::is_same_v<BareT, String>) {
                result = arg.is_constant();
            } else if constexpr (std::is_same_v<BareT, Lambda>) {
                result = arg.is_constant();
            }

            result = true;
        }, m_data);

        return result;
    }
}