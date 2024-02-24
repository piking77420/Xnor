#include "windows/content_browser.hpp"

#include "ImGui/imgui.h"
#include "resource/resource_manager.hpp"

#define ASSETS_PATH "assets/editor/content_browser/"

using namespace XnorEditor;

ContentBrowser::ContentBrowser(Editor* editor, XnorCore::Pointer<XnorCore::Directory>&& rootDirectory)
    : UiWindow(editor, "Content Browser")
    , m_RootDirectory(std::move(rootDirectory))
    , m_CurrentDirectory(m_RootDirectory)
{
    m_UnknownFileTypeTexture = XnorCore::ResourceManager::Load<XnorCore::Texture>(XnorCore::FileManager::Get(ASSETS_PATH "unknown_file_type.png"));
}

void ContentBrowser::Display()
{
    FetchInfo();

    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("##left", ImVec2(available.x * 0.5f, 0.f), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    DisplayEntry(static_cast<XnorCore::Pointer<XnorCore::Entry>>(m_RootDirectory));
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##right");
    
    const std::string title = m_CurrentDirectory->GetPath().generic_string();
    const char* const titleStr = title.c_str();
    //available = ImGui::GetContentRegionAvail();
    const ImVec2 textSize = ImGui::CalcTextSize(titleStr);
    XnorCore::Utils::CenterImguiObject(textSize.x);
    ImGui::Text("%s", titleStr);
    // TODO Add child window around title for visual border

    for ([[maybe_unused]] const XnorCore::Pointer<XnorCore::Entry>& entry : m_CurrentDirectory->GetChildEntries())
    {
        ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_UnknownFileTypeTexture->GetId()), ImVec2(64.f, 64.f));
    }
    
    ImGui::EndChild();
}

const XnorCore::Pointer<XnorCore::Directory>& ContentBrowser::GetRootDirectory() const
{
    return m_RootDirectory;
}

void ContentBrowser::SetRootDirectory(const XnorCore::Pointer<XnorCore::Directory>& rootDirectory)
{
    m_RootDirectory = rootDirectory;

    // Make sure m_CurrentDirectory is not a parent directory of m_RootDirectory
    const std::string currentDirRelativeToRoot = relative(m_CurrentDirectory->GetPath(), m_RootDirectory->GetPath()).string();
    if (currentDirRelativeToRoot.find("..") != std::string::npos)
        m_CurrentDirectory = m_RootDirectory;
}

void ContentBrowser::DisplayEntry(const XnorCore::Pointer<XnorCore::Entry>& entry)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;

    const XnorCore::Pointer<XnorCore::Directory> directory = XnorCore::Utils::DynamicPointerCast<XnorCore::Directory>(entry);
    const bool isDirectory = directory;
    
    if (!isDirectory)
        flags |= ImGuiTreeNodeFlags_Leaf;
    else if (entry == m_CurrentDirectory)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(entry->GetName().c_str(), flags))
    {
        if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            m_CurrentDirectory = directory;
        
        if (isDirectory)
        {
            for (const XnorCore::Pointer<XnorCore::Entry>& childEntry : directory->GetChildEntries())
                DisplayEntry(childEntry);
        }
        ImGui::TreePop();
    }
    else if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        m_CurrentDirectory = directory;
    }
}
