#pragma once

#include "definitions.hpp"
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
    XnorCore::SkinnedMeshRenderer* m_SkinnedRenderer = nullptr;

    XnorCore::Viewport m_Viewport;
    XnorCore::Scene m_Scene;
    XnorCore::Entity* m_Entity;
};

END_XNOR_EDITOR
