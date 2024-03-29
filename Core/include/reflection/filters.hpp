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

    XNOR_ENGINE static void BeginResourceFilter(const void* target);
    XNOR_ENGINE static void BeginEntityFilter(const void* target);
    XNOR_ENGINE static void BeginComponentFilter(const void* target);

private:
    static inline const void* m_ResourceFilterTarget = nullptr;
    static inline const void* m_EntityFilterTarget = nullptr;
    static inline const void* m_ComponentFilterTarget = nullptr;
    static inline ImGuiTextFilter m_TextFilter;
};

END_XNOR_CORE

#include "reflection/filters.inl"
