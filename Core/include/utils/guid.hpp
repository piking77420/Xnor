#pragma once

#include <cstdint>
#include <format>
#include <sstream>
#include <string>

#include "core.hpp"

/// @file guid.hpp
/// @brief Defines the Guid class.

BEGIN_XNOR_CORE

/// @brief Stands for Global Unique Identifier, it represents a unique ID that's used to link pointers during serialization and deserialization
class XNOR_ENGINE Guid final
{
public:
    constexpr Guid() = default;

    /// @brief Creates a new @ref Guid
    /// @return New guid
    static Guid New();

    /// @brief Compares 2 @ref Guid
    /// @param other Other guid
    /// @return Whether the @ref Guid are equals
    [[nodiscard]]
    bool_t operator==(const Guid& other) const;

    /// @brief Converts a @ref Guid to a string representation
    explicit operator std::string() const;

private:
    static constexpr inline size_t Data4Size = 8;
    
    uint32_t m_Data1 = 0;
    uint16_t m_Data2 = 0;
    uint16_t m_Data3 = 0;
    uint8_t  m_Data4[Data4Size] = {};
};

END_XNOR_CORE

template <>
struct std::formatter<XnorCore::Guid>
{
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx);

    template <class FormatContext>
    typename FormatContext::iterator format(const XnorCore::Guid& guid, FormatContext& ctx) const;
};

// ReSharper disable once CppMemberFunctionMayBeStatic
template <class FormatContext>
typename FormatContext::iterator std::formatter<XnorCore::Guid>::format(
    const XnorCore::Guid& guid,
    FormatContext& ctx
) const
{
    std::ostringstream out;

    out << static_cast<std::string>(guid);
        
    return std::ranges::copy(std::move(out).str(), ctx.out()).out;
}

template <class ParseContext>
constexpr typename ParseContext::iterator std::formatter<XnorCore::Guid>::parse(ParseContext& ctx)
{
    auto it = ctx.begin();
    if (it == ctx.end())
        return it;
 
    if (*it != '}')
        throw std::format_error("Invalid format args for XnorCore::Guid");
 
    return it;
}
