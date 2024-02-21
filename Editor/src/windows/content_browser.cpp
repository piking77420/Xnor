#include "windows/content_browser.hpp"

#include "ImGui/imgui.h"

using namespace XnorEditor;

ContentBrowser::ContentBrowser(Editor* editor,const std::string& name, std::filesystem::path&& rootDirectory)
    : UiWindow(editor,name)
    , m_RootDirectory(std::move(rootDirectory))
    , m_CurrentDirectory(m_RootDirectory)
{
    CheckRootDirectory();
}

void ContentBrowser::Display()
{
    
    FetchInfo();

    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("##left", ImVec2(available.x * 0.5f, 0.f), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    DisplayPath(m_RootDirectory);
    ImGui::EndChild();

    ImGui::SameLine();
    
    ImGui::BeginChild("##right");
    ImGui::Text("Browse resources here");
    ImGui::EndChild();
    
}

const std::filesystem::path& ContentBrowser::GetRootDirectory() const
{
    return m_RootDirectory;
}

void ContentBrowser::SetRootDirectory(const std::filesystem::path& rootDirectory)
{
    m_RootDirectory = rootDirectory;

    // Make sure m_CurrentDirectory is not a parent directory of m_RootDirectory
    const std::string currentDirRelativeToRoot = relative(m_CurrentDirectory, m_RootDirectory).string();
    if (currentDirRelativeToRoot.find("..") != std::string::npos)
        m_CurrentDirectory = m_RootDirectory;
    
    CheckRootDirectory();
}

void ContentBrowser::CheckRootDirectory() const
{
    if (!is_directory(m_RootDirectory))
        throw std::invalid_argument("ContentBrowser root directory does not point to a directory");
}

void ContentBrowser::DisplayPath(const std::filesystem::path& path)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;

    const bool directory = is_directory(path);
    
    if (!directory)
        flags |= ImGuiTreeNodeFlags_Leaf;
    else if (path == m_CurrentDirectory)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(path.filename().string().c_str(), flags))
    {
        if (directory && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            m_CurrentDirectory = path;
        
        if (directory)
        {
            for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(path))
                DisplayPath(file);
        }
        ImGui::TreePop();
    }
    else if (directory && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        m_CurrentDirectory = path;
    }
}
