#include "pch.hpp"

TEST(Pointer, Basics)
{
    const Pointer<int> ptr(0);

    EXPECT_EQ(ptr.GetReferenceCounter()->GetStrong(), 1);
    EXPECT_EQ(ptr.GetReferenceCounter()->GetWeak(), 0);

    EXPECT_NE(static_cast<int*>(ptr), nullptr);
}

TEST(Pointer, WeakReferences)
{
    Pointer<int> weakRef;

    {
        const Pointer<int> ptr(2);

        weakRef = ptr;

        EXPECT_EQ(ptr.GetReferenceCounter()->GetStrong(), 1);
        EXPECT_EQ(weakRef.GetReferenceCounter()->GetStrong(), 1);

        EXPECT_EQ(ptr.GetReferenceCounter()->GetWeak(), 1);
        EXPECT_EQ(weakRef.GetReferenceCounter()->GetWeak(), 1);

        EXPECT_EQ(static_cast<int*>(weakRef), static_cast<int*>(ptr));

        {
            const Pointer<int> copy = ptr;

            EXPECT_EQ(ptr.GetReferenceCounter()->GetStrong(), 1);
            EXPECT_EQ(ptr.GetReferenceCounter()->GetWeak(), 2);

            EXPECT_EQ(copy.GetReferenceCounter()->GetStrong(), 1);
            EXPECT_EQ(copy.GetReferenceCounter()->GetWeak(), 2);
        }

        EXPECT_EQ(ptr.GetReferenceCounter()->GetStrong(), 1);
        EXPECT_EQ(ptr.GetReferenceCounter()->GetWeak(), 1);

        EXPECT_TRUE(weakRef.IsValid());
    }

    EXPECT_FALSE(weakRef.IsValid());
}

TEST(Pointer, StrongReferences)
{
    Pointer<int> strongRef;

    {
        const Pointer<int> ptr(-17);

        strongRef = ptr.CreateStrongReference();

        EXPECT_EQ(ptr.GetReferenceCounter()->GetStrong(), 2);
        EXPECT_EQ(strongRef.GetReferenceCounter()->GetStrong(), 2);

        EXPECT_EQ(ptr.GetReferenceCounter()->GetWeak(), 0);
        EXPECT_EQ(strongRef.GetReferenceCounter()->GetWeak(), 0);

        EXPECT_EQ(static_cast<int*>(strongRef), static_cast<int*>(ptr));

        EXPECT_TRUE(strongRef.IsValid());
    }
    
    EXPECT_EQ(strongRef.GetReferenceCounter()->GetStrong(), 1);

    EXPECT_TRUE(strongRef.IsValid());
}
