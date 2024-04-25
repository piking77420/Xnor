#include "windows/animation_montage_window.hpp"
#include "resource/animation_montage.hpp"

using namespace XnorEditor;

AnimationMontageWindow::AnimationMontageWindow(Editor* editor, XnorCore::AnimationMontage* montage)
    : UiWindow(editor, "AnimationMontageEditor"), m_Montage(montage)
{
}

void AnimationMontageWindow::Display()
{
}

void AnimationMontageWindow::SetParam(void* param)
{
    m_Montage = static_cast<decltype(m_Montage)>(param);
}
