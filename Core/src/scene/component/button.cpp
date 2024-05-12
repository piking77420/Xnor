#include "scene/component/button.hpp"

#include "input/input.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

void Button::Update()
{
    Component::Update();
    componentTarget = entity->GetComponent<Image>();
    ButtonUpdate();
}

void Button::ButtonUpdate() const
{
    if (!Input::GetMouseButton(MouseButton::Left))
        return;

    if (componentTarget == nullptr)
        return;

    const Vector2 mousePosApplication = Input::GetMousePosition<Vector2>();
    
    std::vector<Input::BindedWindowInfo> windowBindedInfos;
    Input::GetWindowBindedInfo(&windowBindedInfos);

    // Look for each window if we ckeck on the boutton bounding box
    for (const Input::BindedWindowInfo& windowInfo : windowBindedInfos)
    {
        if (windowInfo.isEditor)
            continue;
        
        const Vector2 mousePosWindow = mousePosApplication - windowInfo.windowPos;

        const Vector2 halfSize = componentTarget->size;
        
        const bool_t xAxis = mousePosWindow.x >= (componentTarget->screenTransform.x - halfSize.x) && 
                       mousePosWindow.x <= (componentTarget->screenTransform.x + halfSize.x);
        
        const bool_t yAxis = mousePosWindow.y >= (componentTarget->screenTransform.y - halfSize.y) &&
                     mousePosWindow.y <= (componentTarget->screenTransform.y + halfSize.y);
        
        if (yAxis && xAxis)
        {
            onClick.Invoke();
        }
        
    }
}
