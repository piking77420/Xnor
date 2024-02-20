#include "windows/scene_graph.hpp"

#include "world.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "scene/scene.hpp"

using namespace XnorEditor;

void SceneGraph::Display()
{
    ImGui::Begin("Scene graph");

    // TODO fetch current loaded scene
    XnorCore::Scene& scene = XnorCore::World::world->Scene;
  
    const std::vector<XnorCore::Entity*>& entities = scene.GetEntities();

    if (ImGui::TreeNodeEx("Entities", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::Selectable("Add entity"))
            {
                XnorCore::World::world->Scene.CreateEntity("Entity", nullptr);
            }

            ImGui::EndPopup();
        }
        
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("SG");
            
            if (payload)
            {
                XnorCore::Entity* const dragged = *static_cast<XnorCore::Entity**>(payload->Data);

                dragged->SetParent(nullptr);
            }
            
            ImGui::EndDragDropTarget();
        }

        for (size_t i = 0; i < entities.size(); i++)
        {
            if (!entities[i]->HasParent())
                DisplayEntity(entities[i]);
        }
        
        ImGui::TreePop();
    }

    ImGui::End();

    if (m_EntityToDelete)
    {
        scene.DestroyEntity(m_EntityToDelete);
        m_EntityToDelete = nullptr;

        if (!scene.HasEntity(m_Editor->data.selectedEntity))
            m_Editor->data.selectedEntity = nullptr;
    }
}

void SceneGraph::DisplayEntity(XnorCore::Entity* const entity)
{
    ImGui::PushID(entity);
    
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (!entity->HasChildren())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (m_Editor->data.selectedEntity == entity)
        flags |= ImGuiTreeNodeFlags_Selected;
    
    if (ImGui::TreeNodeEx(entity->name.c_str(), flags))
    {
        if (ImGui::IsItemClicked())
        {
            m_Editor->data.selectedEntity = entity;
        }
        
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::Selectable("Add empty child"))
            {
                XnorCore::World::world->Scene.CreateEntity("Entity", entity);                
            }
            
            ImGui::Selectable("Rename");          

            if (ImGui::Selectable("Delete"))
            {
                m_EntityToDelete = entity;
            }

            ImGui::EndPopup();
        }
        
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("SG", &entity, sizeof(entity));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("SG");
            
            if (payload)
            {
                XnorCore::Entity* const dragged = *static_cast<XnorCore::Entity**>(payload->Data);

                if (!dragged->IsAParentOf(entity))
                {
                    dragged->SetParent(entity);
                }
            }
            
            ImGui::EndDragDropTarget();
        }
        
        for (size_t i = 0; i < entity->GetChildCount(); i++)
        {
            DisplayEntity(entity->GetChild(i));
        }

        ImGui::TreePop();
    }
    else if (ImGui::IsItemClicked())
    {
        m_Editor->data.selectedEntity = entity;
    }

    ImGui::PopID();
}
