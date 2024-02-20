#pragma once

#include <vector>

#include "definitions.hpp"
#include "applications/application.hpp"

BEGIN_XNOR_EDITOR

class UiWindow;

class Editor : public XnorCore::Application
{
public:
	explicit Editor();

	DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(Editor)

	~Editor() override;
	
	void Update() override;
	
private:
	std::vector<UiWindow*> m_UiWindows;

	void BeginFrame();

	void EndFrame();

private:
	void CreateDefaultWindows();
	
	void BeginDockSpace() const;

	void EndDockSpace() const;
		
	void SetupImGuiStyle() const;
};

END_XNOR_EDITOR
