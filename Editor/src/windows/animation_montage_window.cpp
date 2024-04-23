#include "windows/animation_montage_window.hpp"
#include "resource/animation_montage.hpp"

using namespace XnorEditor;

AnimationMontageWindow::AnimationMontageWindow(Editor* editor, XnorCore::AnimationMontage* montage)
    : UiWindow(editor, "AnimationMontage"), m_Montage(montage)
{
}

void AnimationMontageWindow::Display()
{
}
