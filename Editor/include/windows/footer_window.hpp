#pragma once

#include "definitions.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class FooterWindow : public UiWindow
{
public:
    explicit FooterWindow(Editor* editor);

    void Display() override;

private:
    XnorCore::Pointer<XnorCore::Texture> m_BuildTexture;
};

END_XNOR_EDITOR
