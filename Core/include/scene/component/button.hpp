#pragma once
#include "core.hpp"
#include "gui_component.hpp"
#include "image.hpp"
#include "Maths/vector2i.hpp"

BEGIN_XNOR_CORE
class Button : public GuiComponent
{
public:
    Image* componentTarget { nullptr };

    ~Button() override;

    void Update() override;
    
private:
    
};

END_XNOR_CORE
