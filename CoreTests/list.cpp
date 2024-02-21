#include "pch.h"

using namespace XnorCore;

namespace TestSize
{
	List<int> list;
	
	TEST(List, Size)
	{
		EXPECT_EQ(list.GetSize(), 0);
		list.Add(0);
		EXPECT_EQ(list.GetSize(), 1);
		list.AddRange({ 0, 1, 2, 3 });
		EXPECT_EQ(list.GetSize(), 5);
	}
}

