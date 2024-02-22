#include "pch.h"

TEST(Utils, IntToPointer)
{
    EXPECT_EQ(Utils::IntToPointer<char*>(0), nullptr);
}

TEST(Utils, MetaProgramming)
{
    EXPECT_FALSE(Utils::is_std_vector<bool>::value);
    EXPECT_TRUE(Utils::is_std_vector<std::vector<bool>>::value);
    
    EXPECT_TRUE((std::is_same_v<Utils::ptr_to_void_ptr<bool>::type, bool>));
    EXPECT_TRUE((std::is_same_v<Utils::ptr_to_void_ptr<bool*>::type, void*>));
}

TEST(Utils, ConversionVectorImVec)
{
    constexpr Vector2 v(2.4f);
    EXPECT_EQ(v, Utils::FromImVec(Utils::ToImVec(v)));
}

TEST(Utils, PathToForwardSlashes)
{
    constexpr const char* filepath = __FILE__;
    
    const std::string sPath = filepath;
    const std::filesystem::path pPath = filepath;

    const std::string fsPath = Utils::PathToForwardSlashes(sPath);
    const std::string fpPath = Utils::PathToForwardSlashes(pPath);

    EXPECT_FALSE(fsPath.contains('\\'));
    EXPECT_FALSE(fpPath.contains('\\'));

    EXPECT_EQ(fsPath, fpPath);
}

TEST(Utils, HumanizeString)
{
    EXPECT_EQ(Utils::HumanizeString("stringToHumanize"), "String To Humanize");
    EXPECT_EQ(Utils::HumanizeString("RequiresUIReload"), "Requires UI Reload");
}
