#pragma once

#include "core.hpp"
#include "definitions.hpp"
#include "editor.hpp"
#include "Maths/vector2i.hpp"

BEGIN_XNOR_EDITOR

class UiWindow
{
public:
    explicit UiWindow(Editor* editor,const std::string& name);
    virtual ~UiWindow() = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(UiWindow)

    const char* GetName() const;
    
    virtual void Display() = 0;
    
    [[nodiscard]] bool IsFocused() const;
    [[nodiscard]] Vector2i GetPosition() const;
    [[nodiscard]] Vector2i GetSize() const;
    void FetchInfo();

protected:

    std::string m_WindowName = "No Name";
    
    bool m_IsFocused;
    Vector2i m_Position;
    Vector2i m_Size;

    Editor* m_Editor;
};

END_XNOR_EDITOR
