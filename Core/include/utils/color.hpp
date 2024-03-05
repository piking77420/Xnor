#pragma once

#include <cmath>

#include "core.hpp"

// Undef windows min and max macros in case they were defined
#undef min
#undef max

BEGIN_XNOR_CORE

struct ColorRgba;
struct ColorHsv;
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
    constexpr explicit operator ColorHsv() const;
    
    /// @brief Converts the ColorRgb to a Colorf
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

/// @brief The ColorRgba struct represents a color in RGBA color space.
///
/// It uses values from 0 to 255 (0xFF). The default alpha value is 255.
struct XNOR_ENGINE ColorRgba
{
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
    constexpr explicit operator ColorHsv() const;
    
    /// @brief Converts the ColorRgba to a Colorf
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

/// @brief The Colorf struct represents a color in RGBA color space.
/// 
/// It uses values from 0 to 1. The default alpha value is 1.
struct XNOR_ENGINE Colorf
{
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
    constexpr explicit operator ColorHsv() const;
};

/// @brief The ColorHsv struct represents a color in HSVA color space.
///
/// It uses values from 0 to 255 (0xFF). The default alpha value is 255.
struct XNOR_ENGINE ColorHsv
{
    /// @brief Hue component
    uint8_t h = 0;
    /// @brief Saturation component
    uint8_t s = 0;
    /// @brief Value component
    uint8_t v = 0;
    /// @brief Alpha component
    uint8_t a = 0xFF;

    ColorHsv() = default;

    /// @brief Constructs a color using a 32 bit HSVA constant
    /// 
    /// @param hsva H S V A, with each component taking 8 bits and in this order
    [[nodiscard]]
    constexpr explicit ColorHsv(const uint32_t hsva)
        : h(static_cast<uint8_t>(hsva >> 24)), s(static_cast<uint8_t>(hsva >> 16)), v(static_cast<uint8_t>(hsva >> 8)), a(static_cast<uint8_t>(hsva)) {}

    /// @brief Constructs a color with each specified component
    /// 
    /// @param h Hue component
    /// @param s Saturation component
    /// @param v Value component
    /// @param a Alpha component
    [[nodiscard]]
    constexpr ColorHsv(const uint8_t h, const uint8_t s, const uint8_t v, const uint8_t a = 0xFF)
    : h(h), s(s), v(v), a(a) {}

    /// @brief Converts the ColorHsv to a ColorRgba
    [[nodiscard]]
    constexpr explicit operator ColorRgba() const;

    /// @brief Converts the ColorHsv to a ColorRgb
    [[nodiscard]]
    constexpr explicit operator ColorRgb() const;
    
    /// @brief Converts the ColorHsv to a Colorf
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

constexpr ColorRgb ColorRgb::White() { return ColorRgb(0xFF, 0xFF, 0xFF); }

constexpr ColorRgb ColorRgb::Gray() { return ColorRgb(0x7F, 0x7F, 0x7F); }

constexpr ColorRgb ColorRgb::Black() { return ColorRgb(0x00, 0x00, 0x00); }

constexpr ColorRgb ColorRgb::Red() { return ColorRgb(0xFF, 0x00, 0x00); }

constexpr ColorRgb ColorRgb::Green() { return ColorRgb(0x00, 0xFF, 0x00); }

constexpr ColorRgb ColorRgb::Blue() { return ColorRgb(0x00, 0x00, 0xFF); }

constexpr ColorRgb ColorRgb::Yellow() { return ColorRgb(0xFF, 0xFF, 0x00); }

constexpr ColorRgb ColorRgb::Cyan() { return ColorRgb(0x00, 0xFF, 0xFF); }

constexpr ColorRgb ColorRgb::Magenta() { return ColorRgb(0xFF, 0x00, 0xFF); }

/// @brief Hue angle
constexpr uint8_t HueAngle = 43;

constexpr ColorRgb::operator ColorRgba() const { return ColorRgba(r, g, b); }

constexpr ColorRgb::operator ColorHsv() const
{
    ColorHsv hsv = { 0, 0, 0 };
    const uint8_t minVal = std::min(std::min(r, g), b);
    const uint8_t maxVal = std::max(std::max(r, g), b);
    hsv.v = maxVal;
    const uint8_t delta = maxVal - minVal;
    if (delta == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
    }
    else
    {
        hsv.s = 0xFF * delta / maxVal;
        if (r == maxVal)
            hsv.h = 0 + HueAngle * (g - b) / delta;
        else if (g == maxVal)
            hsv.h = HueAngle * 2 - 1 + HueAngle * (b - r) / delta;
        else
            hsv.h = HueAngle * 4 - 1 + HueAngle * (r - g) / delta;
    }
    return hsv;
}

constexpr ColorHsv::operator ColorRgba() const
{
    const uint8_t hi = h / HueAngle;
    const uint8_t f = h % HueAngle * 6;
    const uint8_t p = (v * (0xFF - s) + 0x7F) / 0xFF;
    const uint8_t q = (v * (0xFF - (s * f + 0x7F) / 0xFF) + 0x7F) / 0xFF;
    const uint8_t t = (v * (0xFF - (s * (0xFF - f) + 0x7F) / 0xFF) + 0x7F) / 0xFF;

    switch (hi)
    {
        case 0:
            return { v, t, p, a };
        case 1:
            return { q, v, p, a };
        case 2:
            return { p, v, t, a };
        case 3:
            return { p, q, v, a };
        case 4:
            return { t, p, v, a };
        default:
            return { v, p, q, a };
    }
}

constexpr ColorHsv::operator ColorRgb() const
{
    const uint8_t hi = h / HueAngle;
    const uint8_t f = h % HueAngle * 6;
    const uint8_t p = (v * (0xFF - s) + 0x7F) / 0xFF;
    const uint8_t q = (v * (0xFF - (s * f + 0x7F) / 0xFF) + 0x7F) / 0xFF;
    const uint8_t t = (v * (0xFF - (s * (0xFF - f) + 0x7F) / 0xFF) + 0x7F) / 0xFF;

    switch (hi)
    {
        case 0:
            return { v, t, p };
        case 1:
            return { q, v, p };
        case 2:
            return { p, v, t };
        case 3:
            return { p, q, v };
        case 4:
            return { t, p, v };
        default:
            return { v, p, q };
    }
}

constexpr ColorHsv::operator Colorf() const
{
    const uint8_t hi = h / HueAngle;
    const uint8_t f = h % HueAngle * 6;
    const uint8_t p = (v * (0xFF - s) + 0x7F) / 0xFF;
    const uint8_t q = (v * (0xFF - (s * f + 0x7F) / 0xFF) + 0x7F) / 0xFF;
    const uint8_t t = (v * (0xFF - (s * (0xFF - f) + 0x7F) / 0xFF) + 0x7F) / 0xFF;

    ColorRgba color;
    switch (hi)
    {
        case 0:
            color = { v, t, p, a };
            break;

        case 1:
            color = { q, v, p, a };
            break;

        case 2:
            color = { p, v, t, a };
            break;

        case 3:
            color = { p, q, v, a };
            break;

        case 4:
            color = { t, p, v, a };
            break;

        default:
            color = { v, p, q, a };
            break;
    }

    return static_cast<Colorf>(color);
}

/// @brief Adds 2 @ref ColorRgb, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A + B
[[nodiscard]]
constexpr ColorRgb operator+(const ColorRgb c1, const ColorRgb c2)
{
    const uint16_t r = c1.r + c2.r;
    const uint16_t g = c1.g + c2.g;
    const uint16_t b = c1.b + c2.b;
    
    return ColorRgb(
        r > 0xFF ? 0xFF : static_cast<uint8_t>(r),
        g > 0xFF ? 0xFF : static_cast<uint8_t>(g),
        b > 0xFF ? 0xFF : static_cast<uint8_t>(b)
    );
}

/// @brief Multiplies 2 @ref ColorRgb, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A * B
[[nodiscard]]
constexpr ColorRgb operator*(const ColorRgb c1, const ColorRgb c2)
{
    const uint16_t r = c1.r * c2.r;
    const uint16_t g = c1.g * c2.g;
    const uint16_t b = c1.b * c2.b;

    // Check for overflow before setting the value
    return ColorRgb(
        r > 0xFF ? 0xFF : static_cast<uint8_t>(r),
        g > 0xFF ? 0xFF : static_cast<uint8_t>(g),
        b > 0xFF ? 0xFF : static_cast<uint8_t>(b)
    );
}

/// @brief Adds 2 @ref ColorRgba, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A + B
[[nodiscard]]
constexpr ColorRgba operator+(const ColorRgba c1, const ColorRgba c2)
{
    const uint16_t r = c1.r + c2.r;
    const uint16_t g = c1.g + c2.g;
    const uint16_t b = c1.b + c2.b;
    const uint16_t a = c1.a + c2.a;
    
    return ColorRgba(
        r > 0xFF ? 0xFF : static_cast<uint8_t>(r),
        g > 0xFF ? 0xFF : static_cast<uint8_t>(g),
        b > 0xFF ? 0xFF : static_cast<uint8_t>(b),
        a > 0xFF ? 0xFF : static_cast<uint8_t>(a)
    );
}

/// @brief Multiplies 2 @ref ColorRgba, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A + B
[[nodiscard]]
constexpr ColorRgba operator*(const ColorRgba c1, const ColorRgba c2)
{
    const uint16_t r = c1.r * c2.r;
    const uint16_t g = c1.g * c2.g;
    const uint16_t b = c1.b * c2.b;
    const uint16_t a = c1.a * c2.a;

    // Check for overflow before setting the value
    return ColorRgba(
        r > 0xFF ? 0xFF : static_cast<uint8_t>(r),
        g > 0xFF ? 0xFF : static_cast<uint8_t>(g),
        b > 0xFF ? 0xFF : static_cast<uint8_t>(b),
        a > 0xFF ? 0xFF : static_cast<uint8_t>(a)
    );
}

/// @brief Multiplies the alpha component of a @ref ColorRgba
/// @param color Color
/// @param alphaFactor Alpha factor
/// @return Color.a * AlphaFactor
[[nodiscard]]
constexpr ColorRgba operator*(const ColorRgba color, const float_t alphaFactor)
{
    return ColorRgba(color.r, color.g, color.b, static_cast<uint8_t>(static_cast<float_t>(color.a) * alphaFactor));
}

/// @brief Multiplies 2 @ref Colorf, caps at the max value of the components and doesn't allow overflow
/// @param c1 A
/// @param c2 B
/// @return A * B
[[nodiscard]]
constexpr Colorf operator*(const Colorf& c1, const Colorf& c2)
{
    return Colorf(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a);
}

/// @brief Multiplies the alpha component of a @ref Colorf
/// @param color Color
/// @param alphaFactor Alpha factor
/// @return Color.a * AlphaFactor
[[nodiscard]]
constexpr Colorf operator*(const Colorf color, const float_t alphaFactor)
{
    return Colorf(color.r, color.g, color.b, color.a * alphaFactor);
}

constexpr Colorf::operator ColorRgb() const { return ColorRgb(static_cast<uint8_t>(r) * 255, static_cast<uint8_t>(g) * 255, static_cast<uint8_t>(b) * 255); }

constexpr Colorf::operator ColorRgba() const { return ColorRgba(static_cast<uint8_t>(r) * 255, static_cast<uint8_t>(g) * 255, static_cast<uint8_t>(b) * 255, static_cast<uint8_t>(a) * 255); }

constexpr Colorf::operator ColorHsv() const { return static_cast<ColorHsv>(static_cast<ColorRgb>(*this)); }

constexpr ColorRgb::operator Colorf() const { return Colorf(static_cast<float_t>(r) / 255.f, static_cast<float_t>(g) / 255.f, static_cast<float_t>(b) / 255.f); }

constexpr ColorRgba::operator ColorRgb() const { return ColorRgb(r, g, b); }

constexpr ColorRgba::operator ColorHsv() const
{
    ColorHsv hsv = { 0, 0, 0, a };
    const uint8_t minVal = std::min(std::min(r, g), b);
    const uint8_t maxVal = std::max(std::max(r, g), b);
    hsv.v = maxVal;
    const uint8_t delta = maxVal - minVal;
    if (delta == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
    }
    else
    {
        hsv.s = 0xFF * delta / maxVal;
        if (r == maxVal)
            hsv.h = 0 + HueAngle * (g - b) / delta;
        else if (g == maxVal)
            hsv.h = HueAngle * 2 - 1 + HueAngle * (b - r) / delta;
        else
            hsv.h = HueAngle * 4 - 1 + HueAngle * (r - g) / delta;
    }
    return hsv;
}

constexpr ColorRgba::operator Colorf() const
{
    return Colorf(static_cast<float_t>(r) / 255.f, static_cast<float_t>(g) / 255.f, static_cast<float_t>(b) / 255.f, static_cast<float_t>(a) / 255.f);
}

END_XNOR_CORE
