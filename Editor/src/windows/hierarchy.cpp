#include "windows/hierarchy.hpp"

#include "world/world.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "scene/scene.hpp"

using namespace XnorEditor;

Hierarchy::Hierarchy(Editor* editor)
    : UiWindow(editor, "Scene Graph")
{
    m_EntityToDelete = nullptr;
    m_EntityToRename = nullptr;
}

void Hierarchy::Display()
{
    // TODO fetch current loaded scene
    XnorCore::Scene& scene = XnorCore::World::world->Scene;
  
    const XnorCore::List<XnorCore::Entity*>& entities = scene.GetEntities();

    if (ImGui::TreeNodeEx("Entities", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::Selectable("Add entity"))
                scene.CreateEntity("Entity", nullptr);

            ImGui::EndPopup();
        }
        
        if (ImGui::BeginDragDropTarget())
        {
            // ReSharper disable once CppTooWideScope
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("SG");
            
            if (payload)
            {
                XnorCore::Entity* const dragged = *static_cast<XnorCore::Entity**>(payload->Data);

                dragged->SetParent(nullptr);
            }
            
            ImGui::EndDragDropTarget();
        }

        for (size_t i = 0; i < entities.GetSize(); i++)  // NOLINT(modernize-loop-convert)
        {
            if (!entities[i]->HasParent())
                DisplayEntity(scene, entities[i]);
        }
        
        ImGui::TreePop();
    }
    
    CheckDeleteEntity(scene);
}

void Hierarchy::DisplayEntity(XnorCore::Scene& scene, XnorCore::Entity* const entity)
{
    ImGui::PushID(entity);
    
    const char* name = entity->name.c_str();
    const bool isRenaming = IsRenamingEntity(entity);
    
    const ImGuiTreeNodeFlags flags = GetEntityNodeFlags(entity);
    
    if (isRenaming)
        name = "##renaming";
    
    if (ImGui::TreeNodeEx(name, flags))
    {
        if (isRenaming)
        {
            DisplayEntityRenaming(entity);
        }
        else
        {
            ProcessEntitySelection(entity);
            
            DisplayEntityContextMenu(scene, entity);
            ProcessEntityDragDrop(entity);
        }
        
        for (size_t i = 0; i < entity->GetChildCount(); i++)
            DisplayEntity(scene, entity->GetChild(i));

        ImGui::TreePop();
    }
    else
    {
        ProcessEntitySelection(entity);
    }

    ImGui::PopID();
}

void Hierarchy::DisplayEntityContextMenu(XnorCore::Scene& scene, XnorCore::Entity* const entity)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Add child"))
            scene.CreateEntity("Entity", entity);

        if (ImGui::Selectable("Add parent"))
        {
            XnorCore::Entity* const e = scene.CreateEntity("Entity", entity->GetParent());
            e->AddChild(entity);
        }

        if (ImGui::Selectable("Parent to root"))
            entity->SetParent(nullptr);
                    
        if (ImGui::Selectable("Rename"))
            m_EntityToRename = entity;

        if (ImGui::Selectable("Delete"))
            m_EntityToDelete = entity;

        ImGui::EndPopup();
    }
}

void Hierarchy::DisplayEntityRenaming(XnorCore::Entity* const entity)
{
    ImGui::SameLine();
    ImGui::SetKeyboardFocusHere();

    if (ImGui::InputText("##input", &entity->name, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
        m_EntityToRename = nullptr;

    if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        m_EntityToRename = nullptr;
}

void Hierarchy::ProcessEntityDragDrop(XnorCore::Entity* const entity)
{
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("SG", &entity, sizeof(XnorCore::Entity*));
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        // ReSharper disable once CppTooWideScope
        const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("SG");
                
        if (payload)
        {
            XnorCore::Entity* const dragged = *static_cast<XnorCore::Entity**>(payload->Data);

            if (!dragged->IsAParentOf(entity))
                dragged->SetParent(entity);
        }
                
        ImGui::EndDragDropTarget();
    }
}

void Hierarchy::ProcessEntitySelection(XnorCore::Entity* const entity) const
{
    if (ImGui::IsItemClicked())
        m_Editor->data.selectedEntity = entity;
}

void Hierarchy::ProcessEntityDoubleClick(XnorCore::Scene&, XnorCore::Entity* const)
{
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
        // TODO focus object on camera
    }
}

void Hierarchy::CheckDeleteEntity(XnorCore::Scene& scene)
{
    if (m_EntityToDelete)
    {
        scene.DestroyEntity(m_EntityToDelete);
        m_EntityToDelete = nullptr;

        if (!scene.HasEntity(m_Editor->data.selectedEntity))
            m_Editor->data.selectedEntity = nullptr;
    }
}

ImGuiTreeNodeFlags Hierarchy::GetEntityNodeFlags(const XnorCore::Entity* const entity) const
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (!entity->HasChildren())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (IsEntitySelected(entity) || IsRenamingEntity(entity))
        flags |= ImGuiTreeNodeFlags_Selected;

    return flags;
}

bool Hierarchy::IsRenamingEntity(const XnorCore::Entity* entity) const
{
    return m_EntityToRename == entity;
}

bool Hierarchy::IsEntitySelected(const XnorCore::Entity* entity) const
{
    return m_Editor->data.selectedEntity == entity;
}
