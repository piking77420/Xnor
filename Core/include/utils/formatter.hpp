// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <filesystem>
#include <format>
#include <sstream>

/// @file formatter.hpp
/// @brief Defines template specializations of @c std::formatter for external types.
///
/// @see <a href="https://en.cppreference.com/w/cpp/utility/format/formatter">std::formatter<a>

// These definitions must be in the global namespace

template <>
struct std::formatter<std::filesystem::path>
{
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx);

    template <class FormatContext>
    typename FormatContext::iterator format(const std::filesystem::path& path, FormatContext& ctx) const;
};

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
