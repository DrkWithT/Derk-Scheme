#include <format>
#include "frontend/token.hpp"

namespace DerkScheme::Frontend {
    std::string_view create_token_sv(const Token &token, std::string_view source) noexcept {
        return source.substr(token.begin, token.length);
    }

    std::string stringify_sv(std::string_view origin) noexcept {
        return std::format("{}", origin);
    }

    std::string create_token_info_str(const Token& token, std::string_view source) noexcept {
        return std::format("{}", create_token_sv(token, source));
    }
}