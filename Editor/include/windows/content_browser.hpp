#pragma once

#include <filesystem>

#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR

class ContentBrowser : public UiWindow
{
    static void BeginDragDrop(const XnorCore::Pointer<XnorCore::File>& file);
    
public:
    static constexpr XnorCore::Colorf SelectedEntryColor = XnorCore::Colorf(0.5f, 0.5f, 0.5f);
    static constexpr XnorCore::Colorf HoveredEntryColor = SelectedEntryColor * 0.5f;
    static constexpr uint8_t MaxEntryTextLines = 3;
    
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
    XnorCore::Pointer<XnorCore::Entry> m_HoveredEntry;

    XnorCore::Pointer<XnorCore::Texture> m_DirectoryTexture;
    XnorCore::Pointer<XnorCore::Texture> m_FileTexture;

    void DisplayDirectoryHierarchy(const XnorCore::Pointer<XnorCore::Entry>& entry);

    void DisplayEntry(const XnorCore::Pointer<XnorCore::Entry>& entry, const XnorCore::Pointer<XnorCore::Texture>& texture, bool* hovered, bool
                      * clicked);
};

END_XNOR_EDITOR
