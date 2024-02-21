#include "editor.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "utils/list.hpp"
#include "utils/logger.hpp"

void Assert(const bool cond, const std::string&& name)
{
	if (cond)
	{
		XnorCore::Logger::LogInfo("Test passed : {}", name);
	}
	else
	{
		XnorCore::Logger::LogError("Test failed : {}", name);
	}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;

	Logger::OpenDefaultFile();

	FileManager::LoadDirectoryRecursive("assets");

	{
		List<int> list;

		Assert(list.GetCapacity() == 1, "Capacity 0");

		list.Add(5);

		Assert(list.GetSize() == 1, "Size 1");
		Assert(list.GetCapacity() == 1, "Capacity 1");
		Assert(list[0] == 5, "Value [0] 5");

		list.Add(list[0] - 1);
		list[0]++;

		Assert(list.GetSize() == 2, "Size 2");
		Assert(list.GetCapacity() == 2, "Capacity 2");
		Assert(list[0] == 6, "Value [0] 6");
		Assert(list[1] == 4, "Value [1] 4");

		list.AddRange({ 1, 2, 3, 4, 7 });

		Assert(list.GetSize() == 7, "Size 7");
		Assert(list.GetCapacity() == 8, "Capacity 8");
		Assert(list[0] == 6, "Value [0] 6");
		Assert(list[1] == 4, "Value [1] 4");
		Assert(list[2] == 1, "Value [2] 1");
		Assert(list[3] == 2, "Value [3] 2");
		Assert(list[4] == 3, "Value [4] 3");
		Assert(list[5] == 4, "Value [5] 4");
		Assert(list[6] == 7, "Value [6] 7");

		Assert(list.Contains(6), "Contains 6");
		Assert(!list.Contains(10), "!Contains 10");

		Assert(list.Exists([&](const int* v, size_t i) -> bool
		{
			return *v == static_cast<int>(list.GetSize());
		}), "Size exists");
	}

	Editor editor;

	editor.Update();

	ResourceManager::RemoveAll();

	FileManager::UnloadAll();
	Logger::Stop();
	
	return 0;
}
