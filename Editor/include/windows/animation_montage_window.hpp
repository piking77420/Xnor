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

    explicit AnimationMontageWindow(Editor* editor, XnorCore::AnimationMontage* montage);

    void Display() override;

    void SetParam(void* param) override;

private:
    XnorCore::AnimationMontage* m_Montage;
};

END_XNOR_EDITOR
