#pragma once

#include "core.hpp"
#include "definitions.hpp"
#include "editor.hpp"
#include "Maths/vector2i.hpp"

BEGIN_XNOR_EDITOR

class UiWindow
{
public:
    UiWindow(Editor* editor, std::string title);
    
    virtual ~UiWindow() = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(UiWindow)

    ImGuiWindowFlags_ windowFlags = ImGuiWindowFlags_None;

    [[nodiscard]]
    const char* GetName() const;
    
    virtual void Display() = 0;
    
    [[nodiscard]]
    bool IsFocused() const;
    
    [[nodiscard]]
    Vector2i GetPosition() const;
    
    [[nodiscard]]
    Vector2i GetSize() const;
    
    void FetchInfo();

    // Whenever the window need to render after the update of the world
    virtual void OnApplicationRendering();

protected:
    std::string m_WindowTitle = "Untitled";
    
    bool m_IsFocused;
    Vector2i m_Position;
    Vector2i m_Size;


    Editor* m_Editor;
};

END_XNOR_EDITOR
