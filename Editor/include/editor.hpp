#pragma once

#include <vector>

#include "application.hpp"
#include "definitions.hpp"
#include "editor_data.hpp"
#include "editing/picking_strategy.hpp"
#include "utils/file_system_watcher.hpp"

BEGIN_XNOR_EDITOR

class UiWindow;
class RenderWindow;
class UserInput;

class Editor final : public XnorCore::Application
{
public:
	void CheckWindowResize() override;
	
	EditorData data;
	
	explicit Editor(int32_t argc, const char_t* const* argv);

	~Editor() override;

	DEFAULT_COPY_MOVE_OPERATIONS(Editor)
	
	void Update() override;

	void BuildAndReloadCode();

	void StartPlaying();

	void StopPlaying();

	// TODO temp
	static constexpr const char_t* SerializedScenePath = "assets/scenes/basic_scene.scene.xml";

private:
	std::vector<UiWindow*> m_UiWindows;

    std::filesystem::path m_SerializedScenePath;

	void UpdateWindows();

	void OnRenderingWindow();
	
	void BeginFrame();

	void EndFrame();
	
	void WorldBehaviours();

	void CreateDefaultWindows();
	
	void BeginDockSpace() const;

	void EndDockSpace() const;

	void SetupImGuiStyle() const;
	
	void MenuBar();
};

END_XNOR_EDITOR
