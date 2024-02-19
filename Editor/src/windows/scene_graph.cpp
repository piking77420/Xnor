#include "windows/scene_graph.hpp"

#include "ImGui/imgui.h"
#include "scene/scene.hpp"

void SceneGraph::Display()
{
    ImGui::Begin("Scene graph");

    // TODO fetch current loaded scene
    XnorCore::Scene* const scene = XnorCore::Scene::scene;
    std::vector<XnorCore::Entity*>& entities = scene->GetEntities(); 
    
    for (XnorCore::Entity* e : entities)
    {
        if (!e->HasParent())
            DisplayEntity(e);
    }

    ImGui::End();
}

void SceneGraph::DisplayEntity(XnorCore::Entity* const entity)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding;

    if (!entity->HasChildren())
        flags |= ImGuiTreeNodeFlags_Leaf;
    
    if (ImGui::TreeNodeEx(entity->name.c_str(), flags))
    {
        for (size_t i = 0; i < entity->GetChildCount(); i++)
        {
            DisplayEntity(entity->GetChild(i));
        }

        ImGui::TreePop();
    }
}
