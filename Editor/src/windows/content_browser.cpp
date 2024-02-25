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
    m_FileTexture = XnorCore::ResourceManager::Load<XnorCore::Texture>(XnorCore::FileManager::Get(ASSETS_PATH "file.png"));
    m_DirectoryTexture = XnorCore::ResourceManager::Load<XnorCore::Texture>(XnorCore::FileManager::Get(ASSETS_PATH "directory.png"));
}

void ContentBrowser::Display()
{
    FetchInfo();

    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("##left", ImVec2(available.x * 0.5f, 0.f), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    DisplayDirectoryHierarchy(static_cast<XnorCore::Pointer<XnorCore::Entry>>(m_RootDirectory));
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##right");
    
    const std::string title = m_CurrentDirectory->GetPathString();
    const char* const titleStr = title.c_str();
    //available = ImGui::GetContentRegionAvail();
    const ImVec2 textSize = ImGui::CalcTextSize(titleStr);
    XnorCore::Utils::CenterImguiObject(textSize.x);
    ImGui::Text("%s", titleStr);
    // TODO Add child window around title for visual border

    for ([[maybe_unused]] const XnorCore::Pointer<XnorCore::Directory>& directory : m_CurrentDirectory->GetChildDirectories())
        DisplayEntry(static_cast<XnorCore::Pointer<XnorCore::Entry>>(directory), m_DirectoryTexture);

    for ([[maybe_unused]] const XnorCore::Pointer<XnorCore::File>& file : m_CurrentDirectory->GetChildFiles())
        DisplayEntry(static_cast<XnorCore::Pointer<XnorCore::Entry>>(file), m_FileTexture);
    
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

void ContentBrowser::DisplayDirectoryHierarchy(const XnorCore::Pointer<XnorCore::Entry>& entry)
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
        
        if (ImGui::BeginPopupContextItem())
        {
            //if (ImGui::Selectable("Rename"))

            if (ImGui::Selectable("Open in explorer"))
                XnorCore::Utils::OpenInExplorer(*entry);

            //if (ImGui::Selectable("Delete"))

            ImGui::EndPopup();
        }
        
        if (isDirectory)
        {
            for (const XnorCore::Pointer<XnorCore::Entry>& childEntry : directory->GetChildEntries())
                DisplayDirectoryHierarchy(childEntry);
        }
        ImGui::TreePop();
    }
    else if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        m_CurrentDirectory = directory;
    }
}

void ContentBrowser::DisplayEntry(const XnorCore::Pointer<XnorCore::Entry>& entry, const XnorCore::Pointer<XnorCore::Texture>& texture)
{
    const float_t oldCursorPos = ImGui::GetCursorPosX();

    if (m_SelectedEntry == entry)
        ImGui::PushStyleColor(ImGuiCol_ChildBg, XnorCore::Utils::ToImCol(SelectedEntryColor));
    
    const ImVec2 textSize = ImGui::CalcTextSize(entry->GetName().c_str());
    ImGui::BeginChild(
        entry->GetPathString().c_str(),
        ImVec2(
            std::max(textSize.x, static_cast<float_t>(texture->GetSize().x)),
            static_cast<float_t>(texture->GetSize().y) + textSize.y + ImGui::GetStyle().FramePadding.y * 2.f
        )
    );
    XnorCore::Utils::CenterImguiObject(static_cast<float_t>(texture->GetSize().x));
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(texture->GetId()), XnorCore::Utils::ToImVec(static_cast<Vector2>(texture->GetSize())));
    XnorCore::Utils::CenterImguiObject(textSize.x);
    ImGui::Text("%s", entry->GetName().c_str());
    ImGui::EndChild();
    
    if (m_SelectedEntry == entry)
        ImGui::PopStyleColor();

    if (ImGui::IsItemClicked())
    {
        m_SelectedEntry = entry;

        XnorCore::Pointer<XnorCore::Directory>&& directory = XnorCore::Utils::DynamicPointerCast<XnorCore::Directory>(entry);
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && directory)
        {
            m_CurrentDirectory = std::move(directory);
            m_SelectedEntry = nullptr;
        }
    }

    ImGui::SameLine();
    
    const float_t newCursorPos = ImGui::GetCursorPosX();
    if (newCursorPos + (newCursorPos - oldCursorPos) > ImGui::GetContentRegionAvail().x)
        ImGui::Dummy(ImVec2());
}
