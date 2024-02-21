#include "pch.h"

TEST(Utils, IntToPointer)
{
    EXPECT_EQ(Utils::IntToPointer<char*>(0), nullptr);
}
