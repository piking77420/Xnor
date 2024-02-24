#pragma once

#include <filesystem>

#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR

class ContentBrowser : public UiWindow
{
public:
    static constexpr XnorCore::Colorf SelectedEntryColor = XnorCore::Colorf(0.1f, 0.1f, 1.f, 1.f);
    
    using UiWindow::UiWindow;

    ContentBrowser(Editor* editor, XnorCore::Pointer<XnorCore::Directory>&& rootDirectory);
    
    void Display() override;

    [[nodiscard]]
    const XnorCore::Pointer<XnorCore::Directory>& GetRootDirectory() const;

    void SetRootDirectory(const XnorCore::Pointer<XnorCore::Directory>& rootDirectory);

private:
    XnorCore::Pointer<XnorCore::Directory> m_RootDirectory;
    XnorCore::Pointer<XnorCore::Directory> m_CurrentDirectory;
    
    XnorCore::Pointer<XnorCore::Entry> m_SelectedEntry;

    XnorCore::Pointer<XnorCore::Texture> m_DirectoryTexture;
    XnorCore::Pointer<XnorCore::Texture> m_FileTexture;

    void DisplayDirectoryHierarchy(const XnorCore::Pointer<XnorCore::Entry>& entry);

    void DisplayEntry(const XnorCore::Pointer<XnorCore::Entry>& entry, const XnorCore::Pointer<XnorCore::Texture>& texture);
};

END_XNOR_EDITOR
