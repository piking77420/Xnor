#include "pch.hpp"

template <typename T, typename U>
bool Compare(const T& a, const U& b)
{
    return a == b;
}

TEST(Color, ColorRgbToHsv)
{
    constexpr ColorRgb from = ColorRgb::Magenta();
    constexpr ColorHsva to = static_cast<ColorHsva>(from);

    if (Compare(to, ColorHsva::Magenta()))
    {
        
    }
    
    EXPECT_EQ(to, ColorHsva::Magenta());
}

TEST(Color, ColorRgbEquality)
{
    EXPECT_EQ(ColorRgb::Cyan(), ColorRgb::Cyan());
}

TEST(Color, ColorRgbaEquality)
{
    EXPECT_EQ(ColorRgba::Cyan(), ColorRgba::Cyan());
}

TEST(Color, ColorfEquality)
{
    EXPECT_EQ(Colorf::Cyan(), Colorf::Cyan());
}

TEST(Color, ColorHsvaEquality)
{
    EXPECT_EQ(ColorHsva::Cyan(), ColorHsva::Cyan());
}
