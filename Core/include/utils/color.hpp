#pragma once

#include <cmath>

#include "core.hpp"

// Undef windows min and max macros in case they were defined
#undef min
#undef max

BEGIN_XNOR_CORE

struct ColorHsv;
struct Colorf;

/// @brief The @ref Color struct represents a color in RGBA color space.
///        It uses values from 0 to 255 (0xFF). The default alpha value is 255.
struct XNOR_ENGINE Color
{
    [[nodiscard]]
    static constexpr Color White();
    [[nodiscard]]
    static constexpr Color Gray();
    [[nodiscard]]
    static constexpr Color Black();

    [[nodiscard]]
    static constexpr Color Red();
    [[nodiscard]]
    static constexpr Color Green();
    [[nodiscard]]
    static constexpr Color Blue();

    [[nodiscard]]
    static constexpr Color Yellow();
    [[nodiscard]]
    static constexpr Color Cyan();
    [[nodiscard]]
    static constexpr Color Magenta();

    uint8_t r, g, b, a = 0xFF;

    [[nodiscard]]
    Color() = default;

    [[nodiscard]]
    constexpr explicit Color(const uint32_t rgba)
        : r(static_cast<uint8_t>(rgba >> 24)), g(static_cast<uint8_t>(rgba >> 16)), b(static_cast<uint8_t>(rgba >> 8)), a(static_cast<uint8_t>(rgba)) {}
    
    [[nodiscard]]
    constexpr Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0xFF)
        : r(r), g(g), b(b), a(a) {}

    [[nodiscard]]
    constexpr explicit operator ColorHsv() const;
    
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

using ColorRgb = Color;

/// @brief The @ref Colorf struct represents a color in RGBA color space.
///        It uses values from 0 to 1. The default alpha value is 1.
struct XNOR_ENGINE Colorf
{
    float_t r, g, b, a = 1.f;

    [[nodiscard]]
    Colorf() = default;

    [[nodiscard]]
    constexpr Colorf(const float_t r, const float_t g, const float_t b, const float_t a = 1.f)
        : r(r), g(g), b(b), a(a) {}

    [[nodiscard]]
    constexpr explicit operator Color() const;

    [[nodiscard]]
    constexpr explicit operator ColorHsv() const;
};

/// @brief The @ref ColorHSV struct represents a color in HSVA color space.
///        It uses values from 0 to 255 (0xFF). The default alpha value is 255.
struct XNOR_ENGINE ColorHsv
{
    uint8_t h, s, v, a = 0xFF;

    ColorHsv() = default;

    [[nodiscard]]
    constexpr explicit ColorHsv(const uint32_t hsva)
        : h(static_cast<uint8_t>(hsva >> 24)), s(static_cast<uint8_t>(hsva >> 16)), v(static_cast<uint8_t>(hsva >> 8)), a(static_cast<uint8_t>(hsva)) {}
    
    [[nodiscard]]
    constexpr ColorHsv(const uint8_t h, const uint8_t s, const uint8_t v, const uint8_t a = 0xFF)
        : h(h), s(s), v(v), a(a) {}

    [[nodiscard]]
    constexpr explicit operator Color() const;
    
    [[nodiscard]]
    constexpr explicit operator Colorf() const;
};

constexpr Color Color::White() { return Color(0xFF, 0xFF, 0xFF); }

constexpr Color Color::Gray() { return Color(0x7F, 0x7F, 0x7F); }

constexpr Color Color::Black() { return Color(0x00, 0x00, 0x00); }

constexpr Color Color::Red() { return Color(0xFF, 0x00, 0x00); }

constexpr Color Color::Green() { return Color(0x00, 0xFF, 0x00); }

constexpr Color Color::Blue() { return Color(0x00, 0x00, 0xFF); }

constexpr Color Color::Yellow() { return Color(0xFF, 0xFF, 0x00); }

constexpr Color Color::Cyan() { return Color(0x00, 0xFF, 0xFF); }

constexpr Color Color::Magenta() { return Color(0xFF, 0x00, 0xFF); }

constexpr uint8_t HueAngle = 43;

constexpr Color::operator ColorHsv() const
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

constexpr ColorHsv::operator Color() const
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

constexpr ColorHsv::operator Colorf() const
{
    const uint8_t hi = h / HueAngle;
    const uint8_t f = h % HueAngle * 6;
    const uint8_t p = (v * (0xFF - s) + 0x7F) / 0xFF;
    const uint8_t q = (v * (0xFF - (s * f + 0x7F) / 0xFF) + 0x7F) / 0xFF;
    const uint8_t t = (v * (0xFF - (s * (0xFF - f) + 0x7F) / 0xFF) + 0x7F) / 0xFF;

    ColorRgb color;
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

[[nodiscard]]
constexpr Color operator+(const Color c1, const Color c2)
{
    const uint16_t r = c1.r + c2.r;
    const uint16_t g = c1.g + c2.g;
    const uint16_t b = c1.b + c2.b;
    const uint16_t a = c1.a + c2.a;
    
    return Color(
        r > 0xFF ? 0xFF : static_cast<uint8_t>(r),
        g > 0xFF ? 0xFF : static_cast<uint8_t>(g),
        b > 0xFF ? 0xFF : static_cast<uint8_t>(b),
        a > 0xFF ? 0xFF : static_cast<uint8_t>(a)
    );
}

[[nodiscard]]
constexpr Color operator*(const Color c1, const Color c2)
{
    const uint16_t r = c1.r * c2.r;
    const uint16_t g = c1.g * c2.g;
    const uint16_t b = c1.b * c2.b;
    const uint16_t a = c1.a * c2.a;

    // Check for overflow before setting the value
    return Color(
        r > 0xFF ? 0xFF : static_cast<uint8_t>(r),
        g > 0xFF ? 0xFF : static_cast<uint8_t>(g),
        b > 0xFF ? 0xFF : static_cast<uint8_t>(b),
        a > 0xFF ? 0xFF : static_cast<uint8_t>(a)
    );
}

[[nodiscard]]
constexpr Color operator*(const Color color, const float_t alphaFactor) { return Color(color.r, color.g, color.b, static_cast<uint8_t>(static_cast<float_t>(color.a) * alphaFactor)); }

[[nodiscard]]
constexpr Colorf operator*(const Colorf c1, const Colorf c2) { return Colorf(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }

[[nodiscard]]
constexpr Colorf operator*(const Colorf color, const float_t alphaFactor) { return Colorf(color.r, color.g, color.b, color.a * alphaFactor); }

constexpr Colorf::operator Color() const
{
    return Color(static_cast<uint8_t>(r) * 255, static_cast<uint8_t>(g) * 255, static_cast<uint8_t>(b) * 255, static_cast<uint8_t>(a) * 255);
}

constexpr Colorf::operator ColorHsv() const
{
    return static_cast<ColorHsv>(static_cast<ColorRgb>(*this));
}

constexpr Color::operator Colorf() const
{
    return Colorf(static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f, static_cast<float>(a) / 255.f);
}

END_XNOR_CORE
