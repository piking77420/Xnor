#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR
class HeaderWindow : public UiWindow
{
public:
    ~HeaderWindow() = default;
    
    HeaderWindow() = default;
    
    void Display() override;

private:
    
};
END_XNOR_EDITOR