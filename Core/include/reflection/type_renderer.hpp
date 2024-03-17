#pragma once

#include "core.hpp"

#include "resource/resource_manager.hpp"
#include "utils/serializable.hpp"

/// @file type_renderer.hpp
/// @brief Defines the TypeRenderer

BEGIN_XNOR_CORE

/// @brief Helper class to render an object using compile time reflection
class TypeRenderer
{
    STATIC_CLASS(TypeRenderer)
    
public:
    /// @brief Displays an object
    /// @tparam ReflectT Object type
    /// @param obj Object pointer
    template <typename ReflectT>
    static void DisplayObject(ReflectT* obj);
    
private:
    /// @brief Metadata used to process a field
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    struct Metadata
    {
        /// @brief Reflected top level object
        ReflectT* topLevelObj;

        /// @brief Member name
        const char_t* name;
        /// @brief Member object
        MemberT* obj;

        /// @brief Member descriptor
        DescriptorT descriptor;

        /// @brief Range attribute for the member
        const Reflection::Range<MemberT>* range;

        /// @brief Checks if the member has an attribute
        /// @tparam AttributeT Attribute type
        /// @return Has attribute
        template <typename AttributeT>
        [[nodiscard]]
        constexpr bool_t HasAttribute() const
        {
            return Reflection::HasAttribute<AttributeT, DescriptorT>(descriptor);
        }
    };

    /// @brief Displays a simple scalar
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayScalar(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);
    
    /// @brief Displays a math type (Vector and Quaternion)
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayMathType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays a color type
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayColorType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays a raw pointer
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayRawPointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays a Pointer
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayXnorPointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays an enum
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayEnum(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays an enum flag
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayEnumFlag(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayObjectInternal(ReflectT* obj, DescriptorT member);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplaySimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);
    
    /// @brief Displays a native array
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayArray(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays a List
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayList(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays a tooltip if the member has one
    /// @tparam ReflectT Reflected top level type
    /// @tparam MemberT Member type
    /// @tparam DescriptorT Field descriptor type
    /// @param metadata Member metadata
    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void CheckDisplayTooltip(const Metadata<ReflectT, MemberT, DescriptorT>& metadata);

    /// @brief Displays an object via the Factory using its hash
    /// @param obj Object pointer
    /// @param hash Object hash
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
