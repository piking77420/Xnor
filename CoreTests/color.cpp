#include "pch.hpp"

TEST(Color, ConversionRgbaHsva)
{
    {
        constexpr ColorRgba rgba = ColorRgba::Red();
        constexpr ColorHsva hsva = ColorHsva::Red();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<ColorRgba>(hsva), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Green();
        constexpr ColorHsva hsva = ColorHsva::Green();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<ColorRgba>(hsva), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Blue();
        constexpr ColorHsva hsva = ColorHsva::Blue();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<ColorRgba>(hsva), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Yellow();
        constexpr ColorHsva hsva = ColorHsva::Yellow();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<ColorRgba>(hsva), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Magenta();
        constexpr ColorHsva hsva = ColorHsva::Magenta();

        EXPECT_EQ(static_cast<ColorHsva>(rgba), hsva);
        EXPECT_EQ(static_cast<ColorRgba>(hsva), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::LightBlue();
        constexpr ColorHsva hsva = ColorHsva::LightBlue();

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
        constexpr ColorRgba rgba = ColorRgba::Red();
        constexpr Colorf f = Colorf::Red();

        EXPECT_EQ(static_cast<Colorf>(rgba), f);
        EXPECT_EQ(static_cast<ColorRgba>(f), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Green();
        constexpr Colorf f = Colorf::Green();

        EXPECT_EQ(static_cast<Colorf>(rgba), f);
        EXPECT_EQ(static_cast<ColorRgba>(f), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Blue();
        constexpr Colorf f = Colorf::Blue();

        EXPECT_EQ(static_cast<Colorf>(rgba), f);
        EXPECT_EQ(static_cast<ColorRgba>(f), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Yellow();
        constexpr Colorf f = Colorf::Yellow();

        EXPECT_EQ(static_cast<Colorf>(rgba), f);
        EXPECT_EQ(static_cast<ColorRgba>(f), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::Magenta();
        constexpr Colorf f = Colorf::Magenta();

        EXPECT_EQ(static_cast<Colorf>(rgba), f);
        EXPECT_EQ(static_cast<ColorRgba>(f), rgba);
    }
    {
        constexpr ColorRgba rgba = ColorRgba::LightBlue();
        constexpr Colorf f = Colorf::LightBlue();

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
    EXPECT_EQ(ColorRgb::LightBlue(), ColorRgb::LightBlue());
}

TEST(Color, EqualityRgba)
{
    EXPECT_EQ(ColorRgba::LightBlue(), ColorRgba::LightBlue());
}

TEST(Color, Equalityf)
{
    EXPECT_EQ(Colorf::LightBlue(), Colorf::LightBlue());
}

TEST(Color, EqualityHsva)
{
    EXPECT_EQ(ColorHsva::LightBlue(), ColorHsva::LightBlue());
}
