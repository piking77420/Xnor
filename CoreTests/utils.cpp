#include "pch.hpp"

TEST(Utils, IntToPointer)
{
    EXPECT_EQ(Utils::IntToPointer<char*>(0), nullptr);
}

TEST(Utils, MetaProgramming)
{
    EXPECT_FALSE(Utils::IsStdVector<bool>::value);
    EXPECT_TRUE(Utils::IsStdVector<std::vector<bool>>::value);
    
    EXPECT_TRUE((std::is_same_v<Utils::PtrToVoidPtr<bool>::type, bool>));
    EXPECT_TRUE((std::is_same_v<Utils::PtrToVoidPtr<bool*>::type, void*>));
}

TEST(Utils, ConversionVectorImVec)
{
    constexpr Vector2 v(2.4f);
    EXPECT_EQ(v, Utils::FromImVec(Utils::ToImVec(v)));
}

TEST(Utils, HumanizeString)
{
    EXPECT_EQ(Utils::HumanizeString("stringToHumanize"), "String To Humanize");
    EXPECT_EQ(Utils::HumanizeString("RequiresUIReload"), "Requires UI Reload");
}

struct A
{
    int32_t i = 1;

    A() = default;

    explicit A(const int32_t i) : i(i) {}

    virtual ~A() = default;
};

struct B : A
{
    B() : A(2) {}
};

TEST(Utils, DynamicPointerCast)
{
    Pointer<B> b(Construct{});
    EXPECT_EQ(b->i, 2);
    const Pointer<A> a = Utils::DynamicPointerCast<A>(b);
    EXPECT_EQ(a->i, 2);
    b = Utils::DynamicPointerCast<B>(Pointer<A>(Construct{}));
    EXPECT_FALSE(b.IsValid());
}
