#pragma once


#include "window.hpp"

class Editor
{
public:

	Editor(const Window& window);

	~Editor();

	void BeginFrame();

	void EndFrame(const Window& window);

private:

};

