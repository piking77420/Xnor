#pragma once

#include "definitions.hpp"
#include "windows/ui_window.hpp"

#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

class SceneGraph : public UiWindow
{
public:
    using UiWindow::UiWindow;

    explicit SceneGraph(Editor* editor);
    
    void Display() override;

private:
    void DisplayEntity(XnorCore::Scene& scene, XnorCore::Entity* entity);
    void DisplayEntityContextMenu(XnorCore::Scene& scene, XnorCore::Entity* entity);
    void DisplayEntityRenaming(XnorCore::Entity* entity);

    void ProcessEntityDragDrop(XnorCore::Entity* entity);
    void ProcessEntitySelection(XnorCore::Entity* entity) const;
    void ProcessEntityDoubleClik(XnorCore::Scene& scene, XnorCore::Entity* entity);
    
    void CheckDeleteEntity(XnorCore::Scene& scene);

    [[nodiscard]]
    ImGuiTreeNodeFlags GetEntityNodeFlags(const XnorCore::Entity* entity) const;

    [[nodiscard]]
    bool IsRenamingEntity(const XnorCore::Entity* entity) const;
    [[nodiscard]]
    bool IsEntitySelected(const XnorCore::Entity* entity) const;

    XnorCore::Entity* m_EntityToDelete;
    XnorCore::Entity* m_EntityToRename;
};

END_XNOR_EDITOR
