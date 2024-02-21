#pragma once

#include <filesystem>

#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR

class ContentBrowser : public UiWindow
{
public:
    using UiWindow::UiWindow;

    ContentBrowser(Editor* editor,const std::string& name, std::filesystem::path&& rootDirectory);
    
    void Display() override;

    [[nodiscard]]
    const std::filesystem::path& GetRootDirectory() const;

    void SetRootDirectory(const std::filesystem::path& rootDirectory);

private:
    std::filesystem::path m_RootDirectory;
    std::filesystem::path m_CurrentDirectory;

    void CheckRootDirectory() const;

    void DisplayPath(const std::filesystem::path& path);
};

END_XNOR_EDITOR
