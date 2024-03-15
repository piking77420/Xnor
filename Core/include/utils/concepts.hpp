#pragma once

#include "core.hpp"
#include "resource/resource.hpp"

#include "scene/component.hpp"

/// @file concepts.hpp
/// @brief Defines the XnorCore::Concepts namespace which contains useful concepts used in the engine.

BEGIN_XNOR_CORE

/// @namespace Concepts
/// @brief Contains useful general-use concepts.
namespace Concepts
{
    /// @brief A class satisfies the ResourceT concept if it is derived of Resource.
    template <class T>
    concept ResourceT = Meta::IsBaseOf<Resource, T>;

    /// @brief A class satisfies the EntryT concept if it is derived of Entry.
    template <class T>
    concept EntryT = Meta::IsBaseOf<Entry, T>;
    
    /// @brief Concept that forces a type to be a child of Component
    template <class T>
    concept ComponentT = Meta::IsBaseOf<Component, T>;

    /// @brief The Formattable concept requires a type to be formattable.
    ///
    /// A type @p T is considered formattable if a template specialization of the @c std::formatter for @p T exists.
    ///
    /// @tparam T The type to require to be formattable.
    template<typename T>
    concept FormattableT = requires(T& v, std::format_context ctx)
    {
        std::formatter<std::remove_cvref_t<T>>().format(v, ctx);
    };

    /// @brief A class satisfies the ExceptionT concept if it is derived of Exception.
    template<typename T>
    concept ExceptionT = std::is_base_of_v<std::exception, T>;

    /// @brief Concept that forces a type to be a function
    template <class T>
    concept FunctionT = std::is_function_v<T>;
}

END_XNOR_CORE
