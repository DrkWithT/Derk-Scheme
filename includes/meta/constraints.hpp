#pragma once

#include <concepts>
#include "semantics/tags.hpp"

namespace DerkScheme::Meta {
    template <typename T>
    concept ValueKind = requires(T arg)
    {
        {auto(arg.type())} -> std::same_as<Semantics::BasicTypeTag>;
        {auto(arg.as_truthy())} -> std::same_as<bool>;
        {auto(arg.is_constant())} -> std::same_as<bool>;
    };
}