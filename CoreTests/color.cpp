#include "pch.hpp"

TEST(Color, ConversionRgbaHsva)
{
    {
        constexpr ColorRgba rgba = ColorRgba::Magenta();
        constexpr ColorHsva hsva = ColorHsva::Magenta();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<ColorRgba>(hsva), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Gray();
        constexpr ColorHsva hsva = ColorHsva::Gray();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<ColorRgba>(hsva), rgba);
    }
}

TEST(Color, ConversionRgbaf)
{
    {
        constexpr ColorRgba rgba = ColorRgba::Magenta();
        constexpr Colorf f = Colorf::Magenta();

        EXPECT_EQ(static_cast<Colorf>(rgba), f);
        EXPECT_EQ(static_cast<ColorRgba>(f), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Gray();
        constexpr Colorf f = Colorf::Gray();

        EXPECT_EQ(static_cast<Colorf>(rgba), f);
        EXPECT_EQ(static_cast<ColorRgba>(f), rgba);
    }
}

TEST(Color, EqualityRgb)
{
    EXPECT_EQ(ColorRgb::Cyan(), ColorRgb::Cyan());
}

TEST(Color, EqualityRgba)
{
    EXPECT_EQ(ColorRgba::Cyan(), ColorRgba::Cyan());
}

TEST(Color, Equalityf)
{
    EXPECT_EQ(Colorf::Cyan(), Colorf::Cyan());
}

TEST(Color, EqualityHsva)
{
    EXPECT_EQ(ColorHsva::Cyan(), ColorHsva::Cyan());
}
