#pragma once

#include "core.hpp"

#include <ImGui/imgui.h>

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

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

    [[nodiscard]]
    const std::string& GetName() const;
    
    virtual void Display() = 0;

    virtual void SetParam([[maybe_unused]] void* const param) {}

    // Whenever the window need to render after the update of the world
    virtual void OnApplicationRendering();
    
    [[nodiscard]]
    bool_t IsFocused() const;
    
    [[nodiscard]]
    Vector2i GetPosition() const;
    
    [[nodiscard]]
    Vector2i GetSize() const;
    
    void FetchInfo();

    bool_t opened = true;
    bool_t canClose = false;

protected:
    std::string m_WindowTitle = "Untitled";
    
    bool_t m_IsFocused;
    Vector2i m_Position;
    Vector2i m_Size;

    Editor* m_Editor;
};

END_XNOR_EDITOR
