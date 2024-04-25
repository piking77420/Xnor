#pragma once

#include <vector>

#include "application.hpp"
#include "definitions.hpp"
#include "editor_data.hpp"
#include "resource/animation_montage.hpp"

BEGIN_XNOR_EDITOR

class UiWindow;
class RenderWindow;
class UserInput;

class Editor final : public XnorCore::Application
{
public:	
	explicit Editor(int32_t argc, const char_t* const* argv);

	~Editor() override;

	DEFAULT_COPY_MOVE_OPERATIONS(Editor)
	
	void Update() override;

	void CheckWindowResize() override;

	template <typename T, typename... Args>
	void OpenWindow(Args&&... args);

	template <typename T, typename... Args>
	void SetupWindow(Args&&... args);

	void OpenCreatedWindow(const std::string& name, void* arg) const;

	EditorData data;

	// TODO temp
	static constexpr const char_t* SerializedScenePath = "assets/scenes/basic_scene.scene.xml";

private:
	std::vector<UiWindow*> m_UiWindows;

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

	XnorCore::AnimationMontage montage;
};

#include "editor.inl"

END_XNOR_EDITOR
