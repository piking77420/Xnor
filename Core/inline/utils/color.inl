#pragma once

#include <Maths/vector3.hpp>
#include <Maths/vector4.hpp>

BEGIN_XNOR_CORE

constexpr ColorRgb ColorRgb::White() { return ColorRgb(0xFF, 0xFF, 0xFF); }

constexpr ColorRgb ColorRgb::Gray() { return ColorRgb(0x7F, 0x7F, 0x7F); }

constexpr ColorRgb ColorRgb::Black() { return ColorRgb(0x00, 0x00, 0x00); }

constexpr ColorRgb ColorRgb::Red() { return ColorRgb(0xFF, 0x00, 0x00); }

constexpr ColorRgb ColorRgb::Green() { return ColorRgb(0x00, 0xFF, 0x00); }

constexpr ColorRgb ColorRgb::Blue() { return ColorRgb(0x00, 0x00, 0xFF); }

constexpr ColorRgb ColorRgb::Yellow() { return ColorRgb(0xFF, 0xFF, 0x00); }

constexpr ColorRgb ColorRgb::LightBlue() { return ColorRgb(0x00, 0xFF, 0xFF); }

constexpr ColorRgb ColorRgb::Magenta() { return ColorRgb(0xFF, 0x00, 0xFF); }

static constexpr uint8_t HueCircle = 255;
static constexpr float_t HueCircleOver3 = HueCircle / 3.f;
static constexpr float_t HueCircleOver6 = HueCircle / 6.f;
static constexpr uint8_t HueCircleOver6Rounded = static_cast<uint8_t>(Calc::Round(HueCircleOver6)); // 255 / 6 = 42.5, rounded to 43 here
static constexpr uint8_t HueCircleOver3Rounded = static_cast<uint8_t>(Calc::Round(HueCircleOver3)); // 255 / 3 = 85

constexpr ColorRgb::operator ColorRgba() const { return ColorRgba(r, g, b); }

constexpr ColorRgb::operator Colorf() const { return Colorf(static_cast<float_t>(r) / 255.f, static_cast<float_t>(g) / 255.f, static_cast<float_t>(b) / 255.f); }

constexpr ColorRgb::operator ColorHsva() const { return static_cast<ColorHsva>(static_cast<ColorRgba>(*this)); }

constexpr ColorRgba ColorRgba::White() { return ColorRgba(0xFF, 0xFF, 0xFF); }

constexpr ColorRgba ColorRgba::Gray() { return ColorRgba(0x7F, 0x7F, 0x7F); }

constexpr ColorRgba ColorRgba::Black() { return ColorRgba(0x00, 0x00, 0x00); }

constexpr ColorRgba ColorRgba::Red() { return ColorRgba(0xFF, 0x00, 0x00); }

constexpr ColorRgba ColorRgba::Green() { return ColorRgba(0x00, 0xFF, 0x00); }

constexpr ColorRgba ColorRgba::Blue() { return ColorRgba(0x00, 0x00, 0xFF); }

constexpr ColorRgba ColorRgba::Yellow() { return ColorRgba(0xFF, 0xFF, 0x00); }

constexpr ColorRgba ColorRgba::LightBlue() { return ColorRgba(0x00, 0xFF, 0xFF); }

constexpr ColorRgba ColorRgba::Magenta() { return ColorRgba(0xFF, 0x00, 0xFF); }

constexpr ColorRgba::operator ColorRgb() const { return ColorRgb(r, g, b); }

constexpr ColorRgba::operator ColorHsva() const
{
    ColorHsva hsv = { 0, 0, 0, a };
    const uint8_t minVal = std::min(std::min(r, g), b);
    const uint8_t maxVal = std::max(std::max(r, g), b);
    hsv.v = maxVal;
    const uint8_t delta = maxVal - minVal;
    
    if (delta == 0) // Black
    {
        hsv.h = 0;
        hsv.s = 0;
    }
    else
    {
        hsv.s = 0xFF * delta / maxVal;
        if (r == maxVal)
            hsv.h = HueCircleOver3Rounded * 0 + HueCircleOver6Rounded * (g - b) / (delta - 1);
        else if (g == maxVal)
            hsv.h = HueCircleOver3Rounded * 1 + HueCircleOver6Rounded * (b - r) / (delta - 1);
        else
            hsv.h = HueCircleOver3Rounded * 2 + HueCircleOver6Rounded * (b - r) / (delta - 1);

        if (hsv.h == 0xFF)
            hsv.h = 0;
    }
    return hsv;
}

constexpr ColorRgba::operator Colorf() const { return Colorf(static_cast<float_t>(r) / 255.f, static_cast<float_t>(g) / 255.f, static_cast<float_t>(b) / 255.f, static_cast<float_t>(a) / 255.f); }

constexpr Colorf Colorf::White() { return Colorf(1.f, 1.f, 1.f); }

constexpr Colorf Colorf::Gray() { return Colorf(0.5f, 0.5f, 0.5f); }

constexpr Colorf Colorf::Black() { return Colorf(0.f, 0.f, 0.f); }

constexpr Colorf Colorf::Red() { return Colorf(1.f, 0.f, 0.f); }

constexpr Colorf Colorf::Green() { return Colorf(0.f, 1.f, 0.f); }

constexpr Colorf Colorf::Blue() { return Colorf(0.f, 0.f, 1.f); }

constexpr Colorf Colorf::Yellow() { return Colorf(1.f, 1.f, 0.f); }

constexpr Colorf Colorf::LightBlue() { return Colorf(0.f, 1.f, 1.f); }

constexpr Colorf Colorf::Magenta() { return Colorf(1.f, 0.f, 1.f); }

constexpr Colorf::operator ColorRgb() const { return ColorRgb(static_cast<uint8_t>(r * 255.f), static_cast<uint8_t>(g * 255.f), static_cast<uint8_t>(b * 255.f)); }

constexpr Colorf::operator ColorRgba() const { return ColorRgba(static_cast<uint8_t>(r * 255.f), static_cast<uint8_t>(g * 255.f), static_cast<uint8_t>(b * 255.f), static_cast<uint8_t>(a * 255.f)); }

constexpr Colorf::operator ColorHsva() const { return static_cast<ColorHsva>(static_cast<ColorRgba>(*this)); }

constexpr Colorf::operator Vector3() const { return Vector3(r, g, b); }

constexpr Colorf::operator Vector4() const { return Vector4(r, g, b, a); }

constexpr ColorHsva ColorHsva::White() { return ColorHsva(0x00, 0x00, 0xFF); }

constexpr ColorHsva ColorHsva::Gray() { return ColorHsva(0x00, 0x00, 0x7F); }

constexpr ColorHsva ColorHsva::Black() { return ColorHsva(0x00, 0x00, 0x00); }

constexpr ColorHsva ColorHsva::Red() { return ColorHsva(0x00, 0xFF, 0xFF); }

constexpr ColorHsva ColorHsva::Green() { return ColorHsva(HueCircleOver3Rounded, 0xFF, 0xFF); }

constexpr ColorHsva ColorHsva::Blue() { return ColorHsva(HueCircleOver3Rounded * 2, 0xFF, 0xFF); }

constexpr ColorHsva ColorHsva::Yellow() { return ColorHsva(HueCircleOver6Rounded, 0xFF, 0xFF); }

constexpr ColorHsva ColorHsva::LightBlue() { return ColorHsva(HueCircleOver6Rounded * 3, 0xFF, 0xFF); }

constexpr ColorHsva ColorHsva::Magenta() { return ColorHsva(HueCircleOver6Rounded * 5, 0xFF, 0xFF); }

constexpr ColorHsva::operator ColorRgba() const
{
    if (s == 0) // Gray
        return { v, v, v, a };
    
    const uint8_t hi = static_cast<uint8_t>(static_cast<float_t>(h) / HueCircleOver6);
    const uint8_t f = static_cast<uint8_t>(Calc::Modulo(h, HueCircleOver6)) * 6;
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

constexpr ColorHsva::operator ColorRgb() const { return static_cast<ColorRgb>(static_cast<ColorRgba>(*this)); }

constexpr ColorHsva::operator Colorf() const { return static_cast<Colorf>(static_cast<ColorRgba>(*this)); }

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

constexpr bool_t operator==(const ColorRgb c1, const ColorRgb c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }

constexpr bool_t operator!=(const ColorRgb c1, const ColorRgb c2) { return !(c1 == c2); }

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

constexpr ColorRgba operator*(const ColorRgba color, const float_t alphaFactor) { return ColorRgba(color.r, color.g, color.b, static_cast<uint8_t>(static_cast<float_t>(color.a) * alphaFactor)); }

constexpr bool_t operator==(const ColorRgba c1, const ColorRgba c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a; }

constexpr bool_t operator!=(const ColorRgba c1, const ColorRgba c2) { return !(c1 == c2); }

constexpr Colorf operator+(const Colorf& c1, const Colorf& c2) { return Colorf(std::min(c1.r + c2.r, 1.f), std::min(c1.g + c2.g, 1.f), std::min(c1.b + c2.b, 1.f), std::min(c1.a + c2.a, 1.f)); }

constexpr Colorf operator*(const Colorf& c1, const Colorf& c2) { return Colorf(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a); }

constexpr Colorf operator*(const Colorf color, const float_t alphaFactor) { return Colorf(color.r, color.g, color.b, color.a * alphaFactor); }

constexpr bool_t operator==(const Colorf& c1, const Colorf& c2) { return Calc::Equals(c1.r, c2.r) && Calc::Equals(c1.g, c2.g) && Calc::Equals(c1.b, c2.b) && Calc::Equals(c1.a, c2.a); }

constexpr bool_t operator!=(const Colorf& c1, const Colorf& c2) { return !(c1 == c2); }

constexpr ColorHsva operator*(const ColorHsva color, const float_t alphaFactor) { return ColorHsva(color.h, color.s, color.v, static_cast<uint8_t>(static_cast<float_t>(color.a) * alphaFactor)); }

constexpr bool_t operator==(const ColorHsva c1, const ColorHsva c2) { return c1.h == c2.h && c1.s == c2.s && c1.v == c2.v && c1.a == c2.a; }

constexpr bool_t operator!=(const ColorHsva c1, const ColorHsva c2) { return !(c1 == c2); }

END_XNOR_CORE
