#pragma once

#include "core.hpp"

#include "resource/resource_manager.hpp"
#include "utils/serializable.hpp"

BEGIN_XNOR_CORE

class TypeRenderer
{
public:
    template <typename ReflectT>
    static void DisplayObject(ReflectT* obj);
    
private:
    template <typename MemberT, typename DescriptorT>
    struct Metadata
    {
        const char_t* name;
        MemberT* obj;

        DescriptorT descriptor;
        
        const Reflection::Range<MemberT>* range;
        const Reflection::Tooltip* tooltip;

        template <typename AttributeT>
        [[nodiscard]]
        constexpr bool_t HasAttribute() const
        {
            return Reflection::HasAttribute<AttributeT>(descriptor);
        }
    };

    template <typename MemberT, typename DescriptorT>
    static void DisplayScalar(const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayMathType(const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayColorType(const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayRawPointer(const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayXnorPointer(const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayEnum(const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayEnumFlag(const Metadata<MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayObjectInternal(ReflectT* obj, DescriptorT member);

    template <typename MemberT, typename DescriptorT>
    static void DisplaySimpleType(const Metadata<MemberT, DescriptorT>& metadata);

    template <typename MemberT, typename DescriptorT>
    static void DisplayArray(const Metadata<MemberT, DescriptorT>& metadata);

    template <typename MemberT, typename DescriptorT>
    static void DisplayList(const Metadata<MemberT, DescriptorT>& metadata);

    XNOR_ENGINE static void DisplayObjectUsingFactory(void* obj, size_t hash);
    
    // TODO maybe move elsewhere
    template <Concepts::ResourceT T>
    static Pointer<T> FilterResources(ImGuiTextFilter& filter);

    static inline Entity* FilterEntity(ImGuiTextFilter& filter);

    XNOR_ENGINE static Component* FilterComponent(ImGuiTextFilter& filter);

    static inline void* m_ResourceFilterTarget = nullptr;
    static inline void* m_EntityFilterTarget = nullptr;
    static inline void* m_ComponentFilterTarget = nullptr;
    static inline ImGuiTextFilter m_TextFilter;
};

END_XNOR_CORE

#include "reflection/type_renderer.inl"
