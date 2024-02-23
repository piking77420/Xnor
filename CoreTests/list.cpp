#include "pch.hpp"

namespace TestOffset
{
    TEST(List, Offset)
    {
        EXPECT_EQ(offsetof(List<int>, m_Data), offsetof(List<Transform>, m_Data));
        EXPECT_EQ(offsetof(List<int>, m_Size), offsetof(List<Transform>, m_Size));
        EXPECT_EQ(offsetof(List<int>, m_Capacity), offsetof(List<Transform>, m_Capacity));
    }
}

namespace TestSize
{
    List<int> list;

    TEST(List, Size)
    {
        EXPECT_EQ(list.GetSize(), 0);
        list.Add(0);
        EXPECT_EQ(list.GetSize(), 1);
        list.AddRange({0, 1, 2, 3});
        EXPECT_EQ(list.GetSize(), 5);
    }
}

namespace TestCapacity
{
    List<int> list;

    TEST(List, Capacity)
    {
        EXPECT_EQ(list.GetCapacity(), 1);
        list.Add(0);
        EXPECT_EQ(list.GetCapacity(), 1);
        list.Add(0);
        EXPECT_EQ(list.GetCapacity(), 2);
        list.Add(0);
        EXPECT_EQ(list.GetCapacity(), 4);
        list.AddRange({0, 1, 2, 3});
        EXPECT_EQ(list.GetCapacity(), 8);

        for (size_t i = 0; i < 1 << 4; i++)
            list.Add(static_cast<int>(i + 8));
        EXPECT_EQ(list.GetCapacity(), 1 << 5);
    }
}

namespace TestValues
{
    List<int> list;

    TEST(List, Values)
    {
        EXPECT_THROW(list[0], std::invalid_argument);

        list.Add(0);
        EXPECT_EQ(list[0], 0);
        list[0]++;
        EXPECT_EQ(list[0], 1);
        list.AddRange({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
        EXPECT_EQ(list[0], 1);
        EXPECT_EQ(list[5], 5);
        EXPECT_NO_THROW(list[0]);
    }
}

namespace TestLambda
{
    List<int> list{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    TEST(List, Lambda)
    {
        EXPECT_EQ(list[4], 4);
        list.Iterate([](int* v, size_t idx) -> void
            {
                (*v)++;
            }
        );
        EXPECT_EQ(list[4], 5);
        EXPECT_EQ(list.Exists([&](const int* v, size_t idx) -> bool
        {
            return *v == list.GetSize();
        }), true);
        EXPECT_EQ(list.Exists([&](const int* v, size_t idx) -> bool
        {
            return *v == list.GetSize() + 5;
        }), false);
        EXPECT_EQ(*list.Find([](const int* v, size_t idx) -> bool
        {
            return *v % 2 == 0;
        }), 2);
    }
}
