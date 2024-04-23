#pragma once

#include "animation_montage_window.hpp"
#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR

class Inspector final : public UiWindow
{
public:
    using UiWindow::UiWindow;

    explicit Inspector(Editor* editor);
    
    void Display() override;

    // static constexpr auto TypeWindowBindings = std::make_tuple;
};

END_XNOR_EDITOR
