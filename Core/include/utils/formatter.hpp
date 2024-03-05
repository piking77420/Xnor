// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <filesystem>
#include <format>
#include <sstream>

/// @file formatter.hpp
/// @brief Defines template specializations of @c std::formatter for external types.
///
/// This file also defines the Formattable concept, as it seems it isn't included in the C++23 MSVC STL.
///
/// @see <a href="https://en.cppreference.com/w/cpp/utility/format/formatter">std::formatter<a>
/// @see <a href="https://en.cppreference.com/w/cpp/utility/format/formattable">std::formattable<a>

BEGIN_XNOR_CORE

/// @brief The formattable concept requires a type to be formattable.
///
/// A type @p T is considered formattable if a template specialization of the @c std::formatter for @p T exists.
///
/// @tparam T The type to require to be formattable.
template<typename T>
concept Formattable = requires (T& v, std::format_context ctx)
{
    std::formatter<std::remove_cvref_t<T>>().format(v, ctx);
};

END_XNOR_CORE

// These definitions must be in the std namespace

/// @brief @c std::formatter template specialization for the @c std::filesystem::path type.
template <>
struct std::formatter<std::filesystem::path>
{
    /// @brief Parses the input formatting options.
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx);

    /// @brief Formats a string using the given instance of @c std::filesystem::path, according to the given options in the parse function.
    template <class FormatContext>
    typename FormatContext::iterator format(const std::filesystem::path& path, FormatContext& ctx) const;
};

template<class ParseContext>
constexpr typename ParseContext::iterator std::formatter<std::filesystem::path>::parse(ParseContext& ctx)
{
    auto it = ctx.begin();
    if (it == ctx.end())
        return it;
 
    if (*it != '}')
        throw std::format_error("Invalid format args for std::filesystem::path");
 
    return it;
}

template<class FormatContext>
typename FormatContext::iterator std::formatter<std::filesystem::path>::format(
    const std::filesystem::path& path,
    FormatContext& ctx
) const
{
    std::ostringstream out;

    out << path.generic_string();
        
    return std::ranges::copy(std::move(out).str(), ctx.out()).out;
}
