#pragma once

#include <string>
#include <unordered_map>
#include "meta/constraints.hpp"
#include "evaluator/values.hpp"

namespace DerkScheme::Evaluator {
    class Environment {
    public:
        Environment(Environment* parent_ref) noexcept;

        [[nodiscard]] bool is_global() const noexcept;
        Value& lookup_value(const std::string& key);

        template <typename ValueT> requires (Meta::ValueKind<std::remove_reference_t<ValueT>>)
        void update_value(const std::string& key, ValueT&& value) {
            m_values[key] = value;
        }

    private:
        std::unordered_map<std::string, Value> m_values;
        Environment* m_parent;
    };
}
