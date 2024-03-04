#pragma once

#include <cstdint>
#include <format>
#include <sstream>
#include <string>

#include "core.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Guid final
{
public:
    constexpr Guid() = default;

    static Guid New();

    [[nodiscard]]
    bool operator==(const Guid& other) const;

    explicit operator std::string() const;

private:
    static constexpr inline uint64_t Data4Size = 8;
    
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
