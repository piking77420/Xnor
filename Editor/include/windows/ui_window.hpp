#pragma once

#include "core.hpp"
#include "Maths/vector2i.hpp"

class UiWindow
{
public:
    UiWindow() = default;
    virtual ~UiWindow() = default;
    DEFAULT_COPY_MOVE_OPERATIONS(UiWindow)

    virtual void Display() = 0;
    
    [[nodiscard]] bool IsFocused() const;
    [[nodiscard]] Vector2i GetPosition() const;
    [[nodiscard]] Vector2i GetSize() const;

protected:
    void FetchInfo();
    
    bool m_IsFocused;
    Vector2i m_Position;
    Vector2i m_Size;
};
