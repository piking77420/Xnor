// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppClangTidyCertDcl58Cpp
#pragma once

#include <filesystem>
#include <format>
#include <sstream>

#include "pointer.hpp"
#include "utils/utils.hpp"

// These definitions must be in the global namespace

template<>
struct std::formatter<std::filesystem::path>
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

        out << p.generic_string();
        
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

template<typename T>
struct std::formatter<XnorCore::Pointer<T>>
{
    template<class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        if (it == ctx.end())
            return it;
 
        if (*it != '}')
            throw std::format_error("Invalid format args for Pointer");
 
        return it;
    }
    
    template<class FormatContext>
    typename FormatContext::iterator format(const XnorCore::Pointer<T>& p, FormatContext& ctx) const
    {
        std::ostringstream out;
        
        out << "0x" << reinterpret_cast<void*>(static_cast<T*>(p));
        
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
