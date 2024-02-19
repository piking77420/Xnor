// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <filesystem>
#include <format>
#include <sstream>

// These definitions must be in the global namespace

template<>
struct std::formatter<std::filesystem::path, char>
{
    template<class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        if (it == ctx.end())
            return it;
 
        if (*it != '}')
            throw std::format_error("Invalid format args for std::filesystem::path");
 
        return it;
    }
    
    template<class FormatContext>
    typename FormatContext::iterator format(const std::filesystem::path& p, FormatContext& ctx) const
    {
        std::ostringstream out;
        out << p;
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
