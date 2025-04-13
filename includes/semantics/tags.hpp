#pragma once

namespace DerkScheme::Semantics {
    enum class BasicTypeTag {
        unknown,
        null,
        boolean,
        number_exact,
        number_real,
        string,
        listy,
        callable,
    };

    enum class ListTag {
        def_variable,
        def_lambda,
        block_do,
        expr_datum,
        expr_multiple
    };

    enum class SemanticErrorCode {
        bad_identifier,
        bad_argument_count,
        not_expr,
        not_callable,
        error_other
    };
}