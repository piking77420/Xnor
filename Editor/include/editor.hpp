#pragma once

#include <vector>

#include "application.hpp"
#include "definitions.hpp"
#include "editor_data.hpp"
#include "utils/file_system_watcher.hpp"

BEGIN_XNOR_EDITOR

class UiWindow;
class RenderWindow;
class UserInput;

class Editor final : public XnorCore::Application
{
public:
	EditorData data;

	XnorCore::Event<> onScriptsReloadingBegin;
	XnorCore::Event<> onScriptsReloadingEnd;
	XnorCore::Event<> onSceneSerializationBegin;
	XnorCore::Event<> onSceneSerializationEnd;
	XnorCore::Event<> onSceneDeserializationBegin;
	XnorCore::Event<> onSceneDeserializationEnd;

	explicit Editor(int32_t argc, const char_t* const* argv);

	~Editor() override;

	DEFAULT_COPY_MOVE_OPERATIONS(Editor)
	
	void Update() override;

	void CheckWindowResize() override;

	template <typename T, typename... Args>
	T* OpenWindow(Args&&... args);

	template <typename T, typename... Args>
	T* SetupWindow(Args&&... args);

	void OpenCreatedWindow(const std::string& name, void* arg) const;

	void BuildAndReloadCodeAsync();

	void StartPlaying();

	void StopPlaying();

	/// @brief Serializes the current scene to the given @p filepath. If the path is empty, instead serializes to a temporary file.
	void SerializeScene(const std::string& filepath = "");

	/// @see SerializeScene
	void SerializeSceneAsync(const std::string& filepath = "");

	void DeserializeScene(const std::string& filepath = "");

	void DeserializeSceneAsync(const std::string& filepath = "");

	// TODO temp
	static constexpr const char_t* SerializedScenePath = "assets/scenes/basic_scene.scene.xml";
	
	[[nodiscard]]
	bool_t IsSerializing() const;
	
	[[nodiscard]]
	bool_t IsDeserializing() const;
	
	[[nodiscard]]
	bool_t IsReloadingScripts() const;

	[[nodiscard]]
	bool_t IsGamePlaying() const;

	[[nodiscard]]
	bool_t IsReloadScriptsOnSave() const;

private:
	std::vector<UiWindow*> m_UiWindows;

    std::filesystem::path m_SerializedScenePath;

	bool_t m_Serializing = false;
	bool_t m_Deserializing = false;
	bool_t m_ReloadingScripts = false;

	bool_t m_ReloadScriptsOnSave = true;

	RenderWindow* m_GameWindow = nullptr;

	/// @brief This thread is used both for serializing the scene, deserializing it, and reloading scripts
	std::thread m_CurrentAsyncActionThread;

	bool_t m_GamePlaying = false;

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
