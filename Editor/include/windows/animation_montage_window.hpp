#pragma once

#include "definitions.hpp"
#include "editor_camera.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_CORE

class AnimationMontage;

END_XNOR_CORE

BEGIN_XNOR_EDITOR

class AnimationMontageWindow final : public UiWindow
{
public:
    using UiWindow::UiWindow;

    explicit AnimationMontageWindow(Editor* editor);

    void Display() override;
    void OnApplicationRendering() override;

    void SetParam(void* param) override;

private:
    XnorCore::AnimationMontage* m_Montage = nullptr;
    XnorCore::SkinnedMeshRenderer* m_SkinnedRenderer = new XnorCore::SkinnedMeshRenderer();

    XnorCore::Viewport m_Viewport;
    EditorCamera m_EditorCamera;
    XnorCore::Camera m_Camera;
    XnorCore::Scene m_Scene;
};

END_XNOR_EDITOR
