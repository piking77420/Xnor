#pragma once
#include "core.hpp"
#include "definitions.hpp"
#include "render_window.hpp"

BEGIN_XNOR_EDITOR
class GameWindow : public RenderWindow
{
public:
    
    GameWindow(Editor* editor, XnorCore::Viewport& viewport);
    
    GameWindow(Editor* editor, const std::string& title, XnorCore::Viewport& viewport);

    ~GameWindow() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(GameWindow)
    
    void FetchInfo() override;
};

END_XNOR_EDITOR