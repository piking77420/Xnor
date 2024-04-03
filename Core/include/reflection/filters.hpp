#pragma once

#include "core.hpp"
#include "ImGui/imgui.h"
#include "scene/entity.hpp"

BEGIN_XNOR_CORE

class Filters
{
    STATIC_CLASS(Filters)

public:
    template <Concepts::ResourceT T>
    static Pointer<T> FilterResources(Pointer<T>* target);

    XNOR_ENGINE static Entity* FilterEntity(Entity** target);

    XNOR_ENGINE static Component* FilterComponent(List<Component*>* target);
    XNOR_ENGINE static Component* FilterComponent(Component** target);

    XNOR_ENGINE static void BeginResourceFilter();
    XNOR_ENGINE static void BeginEntityFilter();
    XNOR_ENGINE static void BeginComponentFilter();

private:
    static inline ImGuiTextFilter m_TextFilter;
};

END_XNOR_CORE

#include "reflection/filters.inl"
