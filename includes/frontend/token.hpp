#pragma once

#include <string>
#include <string_view>

namespace DerkScheme::Frontend {
    enum class TokenTag {
        unknown,
        spacing,
        identifier_name,
        special_name,
        literal_boolean,
        literal_number,
        literal_string,
        left_paren,
        right_paren,
        left_bracket,
        right_bracket,
        eof
    };

    struct Token {
        TokenTag tag;
        int begin;
        int length;
        int line;
        int column;
    };

    [[nodiscard]] std::string_view create_token_sv(const Token& token, std::string_view source) noexcept;

    [[nodiscard]] std::string stringify_sv(std::string_view origin) noexcept;

    [[nodiscard]] std::string create_token_info_str(const Token& token, std::string_view source) noexcept;
}