#pragma once

#include <Maths/vector3.hpp>

#include "core.hpp"

// Undef windows min and max macros in case they were defined
#undef min
#undef max

/// @file color.hpp
/// @brief Defines multiple color structs.

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

    /// @brief Gets the RGB component of the color
    /// @return R, G and B in a Vector3
    [[nodiscard]]
    constexpr Vector3 Rgb() const;

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

END_XNOR_CORE

#include "utils/color.inl"
