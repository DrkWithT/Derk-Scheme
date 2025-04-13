#include <stdexcept>
#include <format>
#include "evaluator/environment.hpp"

namespace DerkScheme::Evaluator {
    Environment::Environment(Environment* parent_ref) noexcept
    : m_values {}, m_parent {parent_ref} {}

    bool Environment::is_global() const noexcept {
        return m_parent == nullptr;
    }

    Value& Environment::lookup_value(const std::string& key) {
        if (m_values.contains(key)) {
            return m_values.at(key);
        } else if (not is_global()) {
            return m_parent->lookup_value(key);
        } else {
            throw std::runtime_error {std::format("LookupError: name {} is unbound to any value.", key)};
        }
    }
}