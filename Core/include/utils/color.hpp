#pragma once

#include <Maths/vector3.hpp>
#include <format>

#include "core.hpp"

// Undef windows min and max macros in case they were defined
#undef min
#undef max

/// @file color.hpp
/// @brief Defines multiple color structs.

BEGIN_XNOR_CORE

struct ColorRgba;
struct ColorHsva;
struct Colorf;

/// @brief The ColorRgb struct represents a color in RGB color space.
///
/// It uses values from 0 to 255 (0xFF).
struct XNOR_ENGINE ColorRgb
{
    /// @brief Constant for white
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb White();
    
    /// @brief Constant for gray
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Gray();
    
    /// @brief Constant for black
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Black();
    
    /// @brief Constant for red
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Red();
    
    /// @brief Constant for green
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Green();
    
    /// @brief Constant for blue
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Blue();
    
    /// @brief Constant for yellow
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Yellow();
    
    /// @brief Constant for cyan
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Cyan();
    
    /// @brief Constant for magenta
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgb Magenta();

    /// @brief Red component
    uint8_t r = 0;
    /// @brief Green component
    uint8_t g = 0;
    /// @brief Blue component
    uint8_t b = 0;

    [[nodiscard]]
    ColorRgb() = default;

    /// @brief Constructs a color using a 32 bit RGB constant, ignoring the last 8 bits
    /// 
    /// @param rgb R G B, with each component taking 8 bits and in this order
    [[nodiscard]]
    constexpr explicit ColorRgb(const uint32_t rgb)
        : r(static_cast<uint8_t>(rgb >> 24)), g(static_cast<uint8_t>(rgb >> 16)), b(static_cast<uint8_t>(rgb >> 8)) {}

    /// @brief Constructs a color with each specified component
    /// 
    /// @param r Red component
    /// @param g Green component
    /// @param b Blue component
    [[nodiscard]]
    constexpr ColorRgb(const uint8_t r, const uint8_t g, const uint8_t b)
    : r(r), g(g), b(b) {}

    /// @brief Converts the ColorRgb to a ColorRgba
    [[nodiscard]]
    constexpr explicit operator ColorRgba() const;

    /// @brief Converts the ColorRgb to a ColorHsv
    [[nodiscard]]
    constexpr explicit operator ColorHsva() const;
    
    /// @brief Converts the ColorRgb to a Colorf
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

/// @brief The ColorRgba struct represents a color in RGBA color space.
///
/// It uses values from 0 to 255 (0xFF). The default alpha value is 255.
struct XNOR_ENGINE ColorRgba
{
    /// @brief Constant for white
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba White();
    
    /// @brief Constant for gray
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Gray();
    
    /// @brief Constant for black
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Black();
    
    /// @brief Constant for red
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Red();
    
    /// @brief Constant for green
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Green();
    
    /// @brief Constant for blue
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Blue();
    
    /// @brief Constant for yellow
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Yellow();
    
    /// @brief Constant for cyan
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Cyan();
    
    /// @brief Constant for magenta
    /// @return Color
    [[nodiscard]]
    static constexpr ColorRgba Magenta();
    
    /// @brief Red component
    uint8_t r = 0;
    /// @brief Green component
    uint8_t g = 0;
    /// @brief Blue component
    uint8_t b = 0;
    /// @brief Alpha component
    uint8_t a = 0xFF;

    [[nodiscard]]
    ColorRgba() = default;

    /// @brief Constructs a color using a 32 bit RGBA constant
    /// 
    /// @param rgba R G B A, with each component taking 8 bits and in this order
    [[nodiscard]]
    constexpr explicit ColorRgba(const uint32_t rgba)
        : r(static_cast<uint8_t>(rgba >> 24)), g(static_cast<uint8_t>(rgba >> 16)), b(static_cast<uint8_t>(rgba >> 8)), a(static_cast<uint8_t>(rgba)) {}

    /// @brief Constructs a color with each specified component
    /// 
    /// @param r Red component
    /// @param g Green component
    /// @param b Blue component
    /// @param a Alpha component
    [[nodiscard]]
    constexpr ColorRgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0xFF)
    : r(r), g(g), b(b), a(a) {}

    /// @brief Converts the ColorRgba to a ColorRgb
    [[nodiscard]]
    constexpr explicit operator ColorRgb() const;

    /// @brief Converts the ColorRgba to a ColorHsv
    [[nodiscard]]
    constexpr explicit operator ColorHsva() const;
    
    /// @brief Converts the ColorRgba to a Colorf
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

/// @brief The Colorf struct represents a color in RGBA color space.
/// 
/// It uses values from 0 to 1. The default alpha value is 1.
struct XNOR_ENGINE Colorf
{
    /// @brief Constant for white
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf White();
    
    /// @brief Constant for gray
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Gray();
    
    /// @brief Constant for black
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Black();
    
    /// @brief Constant for red
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Red();
    
    /// @brief Constant for green
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Green();
    
    /// @brief Constant for blue
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Blue();
    
    /// @brief Constant for yellow
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Yellow();
    
    /// @brief Constant for cyan
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Cyan();
    
    /// @brief Constant for magenta
    /// @return Color
    [[nodiscard]]
    static constexpr Colorf Magenta();
    
    /// @brief Red component
    float_t r = 0.f;
    /// @brief Green component
    float_t g = 0.f;
    /// @brief Blue component
    float_t b = 0.f;
    /// @brief Alpha component
    float_t a = 1.f;

    [[nodiscard]]
    Colorf() = default;

    /// @brief Constructs a color with each specified component
    /// 
    /// @param r Red component
    /// @param g Green component
    /// @param b Blue component
    /// @param a Alpha component
    [[nodiscard]]
    constexpr Colorf(const float_t r, const float_t g, const float_t b, const float_t a = 1.f)
    : r(r), g(g), b(b), a(a) {}

    /// @brief Converts the Colorf to a ColorRgb
    [[nodiscard]]
    constexpr explicit operator ColorRgb() const;

    /// @brief Converts the Colorf to a ColorRgba
    [[nodiscard]]
    constexpr explicit operator ColorRgba() const;

    /// @brief Converts the Colorf to a ColorHsv
    [[nodiscard]]
    constexpr explicit operator ColorHsva() const;

    /// @brief Converts the Colorf to a Vector3
    [[nodiscard]]
    constexpr explicit operator Vector3() const;

    /// @brief Converts the Colorf to a Vector4
    [[nodiscard]]
    constexpr explicit operator Vector4() const;
};

/// @brief The ColorHsva struct represents a color in HSVA color space.
///
/// It uses values from 0 to 255 (0xFF). The default alpha value is 255.
struct XNOR_ENGINE ColorHsva
{
    /// @brief Hue component
    uint8_t h = 0;
    /// @brief Saturation component
    uint8_t s = 0;
    /// @brief Value component
    uint8_t v = 0;
    /// @brief Alpha component
    uint8_t a = 0xFF;

    ColorHsva() = default;

    /// @brief Constructs a color using a 32 bit HSVA constant
    /// 
    /// @param hsva H S V A, with each component taking 8 bits and in this order
    [[nodiscard]]
    constexpr explicit ColorHsva(const uint32_t hsva)
        : h(static_cast<uint8_t>(hsva >> 24)), s(static_cast<uint8_t>(hsva >> 16)), v(static_cast<uint8_t>(hsva >> 8)), a(static_cast<uint8_t>(hsva)) {}

    /// @brief Constructs a color with each specified component
    /// 
    /// @param h Hue component
    /// @param s Saturation component
    /// @param v Value component
    /// @param a Alpha component
    [[nodiscard]]
    constexpr ColorHsva(const uint8_t h, const uint8_t s, const uint8_t v, const uint8_t a = 0xFF)
    : h(h), s(s), v(v), a(a) {}

    /// @brief Converts the ColorHsv to a ColorRgb
    [[nodiscard]]
    constexpr explicit operator ColorRgb() const;

    /// @brief Converts the ColorHsv to a ColorRgba
    [[nodiscard]]
    constexpr explicit operator ColorRgba() const;
    
    /// @brief Converts the ColorHsv to a Colorf
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

END_XNOR_CORE

/// @brief Adds 2 ColorRgb, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A + B
[[nodiscard]]
constexpr XnorCore::ColorRgb operator+(XnorCore::ColorRgb c1, XnorCore::ColorRgb c2);

/// @brief Multiplies 2 ColorRgb, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A * B
[[nodiscard]]
constexpr XnorCore::ColorRgb operator*(XnorCore::ColorRgb c1, XnorCore::ColorRgb c2);

/// @brief Adds 2 ColorRgba, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A + B
[[nodiscard]]
constexpr XnorCore::ColorRgba operator+(XnorCore::ColorRgba c1, XnorCore::ColorRgba c2);

/// @brief Multiplies 2 ColorRgba, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A + B
[[nodiscard]]
constexpr XnorCore::ColorRgba operator*(XnorCore::ColorRgba c1, XnorCore::ColorRgba c2);

/// @brief Multiplies the alpha component of a ColorRgba
/// @param color Color
/// @param alphaFactor Alpha factor
/// @return Color.a * alphaFactor
[[nodiscard]]
constexpr XnorCore::ColorRgba operator*(XnorCore::ColorRgba color, float_t alphaFactor);

/// @brief Adds 2 Colorf, caps at @c 1.f
/// @param c1 A
/// @param c2 B
/// @return A + B
[[nodiscard]]
constexpr XnorCore::Colorf operator+(const XnorCore::Colorf& c1, const XnorCore::Colorf& c2);

/// @brief Multiplies 2 Colorf
/// @param c1 A
/// @param c2 B
/// @return A * B
[[nodiscard]]
constexpr XnorCore::Colorf operator*(const XnorCore::Colorf& c1, const XnorCore::Colorf& c2);

/// @brief Multiplies the alpha component of a Colorf
/// @param color Color
/// @param alphaFactor Alpha factor
/// @return Color.a * alphaFactor
[[nodiscard]]
constexpr XnorCore::Colorf operator*(XnorCore::Colorf color, float_t alphaFactor);

/// @brief Multiplies the alpha component of a ColorHsva
/// @param color Color
/// @param alphaFactor Alpha factor
/// @return Color.a * alphaFactor
[[nodiscard]]
constexpr XnorCore::ColorHsva operator*(XnorCore::ColorHsva color, float_t alphaFactor);

#ifndef SWIG
template <>
struct std::formatter<XnorCore::ColorRgb>
{
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx);

    template <class FmtContext>
    typename FmtContext::iterator format(XnorCore::ColorRgb c, FmtContext& ctx) const;

private:
    std::string m_Format;
};

template <class ParseContext>
constexpr typename ParseContext::iterator std::formatter<XnorCore::ColorRgb, char_t>::parse(ParseContext& ctx)
{
    auto it = ctx.begin();
    if (it == ctx.end())
        return it;
 
    if (*it != '}')
        throw std::format_error("Invalid format args for ColorRgb");
 
    return it;
}

template <class FmtContext>
typename FmtContext::iterator std::formatter<XnorCore::ColorRgb>::format(XnorCore::ColorRgb c, FmtContext &ctx) const
{
    std::ostringstream out;

    out << std::vformat("{:" + m_Format + "} ; {:" + m_Format + "} ; {:" + m_Format + '}', std::make_format_args(c.r, c.g, c.b));

    return std::ranges::copy(std::move(out).str(), ctx.out()).out;
}

template <>
struct std::formatter<XnorCore::ColorRgba>
{
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx);

    template <class FmtContext>
    typename FmtContext::iterator format(XnorCore::ColorRgba c, FmtContext& ctx) const;

private:
    std::string m_Format;
};

template <class ParseContext>
constexpr typename ParseContext::iterator std::formatter<XnorCore::ColorRgba, char_t>::parse(ParseContext& ctx)
{
    auto it = ctx.begin();
    if (it == ctx.end())
        return it;
 
    if (*it != '}')
        throw std::format_error("Invalid format args for ColorRgba");
 
    return it;
}

template <class FmtContext>
typename FmtContext::iterator std::formatter<XnorCore::ColorRgba>::format(XnorCore::ColorRgba c, FmtContext &ctx) const
{
    std::ostringstream out;

    out << std::vformat("{:" + m_Format + "} ; {:" + m_Format + "} ; {:" + m_Format + "} ; {:" + m_Format + '}', std::make_format_args(c.r, c.g, c.b, c.a));

    return std::ranges::copy(std::move(out).str(), ctx.out()).out;
}

template <>
struct std::formatter<XnorCore::Colorf>
{
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx);

    template <class FmtContext>
    typename FmtContext::iterator format(XnorCore::Colorf c, FmtContext& ctx) const;

private:
    std::string m_Format;
};

template <class ParseContext>
constexpr typename ParseContext::iterator std::formatter<XnorCore::Colorf, char_t>::parse(ParseContext& ctx)
{
    auto it = ctx.begin();
    if (it == ctx.end())
        return it;
 
    if (*it != '}')
        throw std::format_error("Invalid format args for Colorf");
 
    return it;
}

template <class FmtContext>
typename FmtContext::iterator std::formatter<XnorCore::Colorf>::format(XnorCore::Colorf c, FmtContext &ctx) const
{
    std::ostringstream out;

    out << std::vformat("{:" + m_Format + "} ; {:" + m_Format + "} ; {:" + m_Format + "} ; {:" + m_Format + '}', std::make_format_args(c.r, c.g, c.b, c.a));

    return std::ranges::copy(std::move(out).str(), ctx.out()).out;
}

template <>
struct std::formatter<XnorCore::ColorHsva>
{
    template <class ParseContext>
    constexpr typename ParseContext::iterator parse(ParseContext& ctx);

    template <class FmtContext>
    typename FmtContext::iterator format(XnorCore::ColorHsva c, FmtContext& ctx) const;

private:
    std::string m_Format;
};

template <class ParseContext>
constexpr typename ParseContext::iterator std::formatter<XnorCore::ColorHsva, char_t>::parse(ParseContext& ctx)
{
    auto it = ctx.begin();
    if (it == ctx.end())
        return it;
 
    if (*it != '}')
        throw std::format_error("Invalid format args for ColorHsva");
 
    return it;
}

template <class FmtContext>
typename FmtContext::iterator std::formatter<XnorCore::ColorHsva>::format(XnorCore::ColorHsva c, FmtContext &ctx) const
{
    std::ostringstream out;

    out << std::vformat("{:" + m_Format + "} ; {:" + m_Format + "} ; {:" + m_Format + "} ; {:" + m_Format + '}', std::make_format_args(c.h, c.s, c.v, c.a));

    return std::ranges::copy(std::move(out).str(), ctx.out()).out;
}
#endif

#include "utils/color.inl"
