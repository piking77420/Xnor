#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR

class Inspector : public UiWindow
{
public:
    using UiWindow::UiWindow;

    explicit Inspector(Editor* editor);
    
    void Display() override;
};

END_XNOR_EDITOR
