#pragma once

#include <filesystem>

#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR

class ContentBrowser : public UiWindow
{
public:
    using UiWindow::UiWindow;

    ContentBrowser(Editor* editor, XnorCore::Pointer<XnorCore::Directory>&& rootDirectory);
    
    void Display() override;

    [[nodiscard]]
    const XnorCore::Pointer<XnorCore::Directory>& GetRootDirectory() const;

    void SetRootDirectory(const XnorCore::Pointer<XnorCore::Directory>& rootDirectory);

private:
    XnorCore::Pointer<XnorCore::Directory> m_RootDirectory;
    XnorCore::Pointer<XnorCore::Directory> m_CurrentDirectory;

    void CheckRootDirectory() const;

    void DisplayEntry(const XnorCore::Pointer<XnorCore::Entry>& entry);
};

END_XNOR_EDITOR
