#include "windows/game_window.hpp"

using namespace XnorEditor;

GameWindow::GameWindow(Editor* editor, XnorCore::Viewport& viewport)
    : RenderWindow(editor,viewport)
{
   
}

GameWindow::GameWindow(Editor* editor, const std::string& title, XnorCore::Viewport& viewport)
    : RenderWindow(editor,title,viewport)
{
}

void GameWindow::FetchInfo()
{
    RenderWindow::FetchInfo();

    // Lock The Game Input whenever the window is focus
    XnorCore::Input::LockInput(!IsFocused());
}
