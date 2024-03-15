#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "resource/resource_manager.hpp"
#include "utils/serializable.hpp"

BEGIN_XNOR_EDITOR

class Inspector : public UiWindow
{
public:
    using UiWindow::UiWindow;

    explicit Inspector(Editor* editor);
    
    void Display() override;

private:
    enum InspectorFlags : size_t
    {
        None = 0 << 0,
        EnumFlag = 1 << 0,
    };

    template <typename MemberT, typename DescriptorT>
    struct Metadata
    {
        size_t flags;
        const char_t* name;

        DescriptorT descriptor;
        
        const XnorCore::Reflection::Range<MemberT>* range;
        const XnorCore::Reflection::Tooltip* tooltip;
    };


    template <typename MemberT, typename DescriptorT>
    static void DisplayScalar(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayMathType(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayColorType(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayRawPointer(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayXnorPointer(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayPolyPointer(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayEnum(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);
    template <typename MemberT, typename DescriptorT>
    static void DisplayEnumFlag(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);

    template <typename ReflectT>
    static void DisplayObject(ReflectT* obj, XnorCore::TypeDescriptor<ReflectT> desc);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayObjectInternal(ReflectT* obj, DescriptorT member);

    template <typename MemberT, typename DescriptorT>
    static void DisplaySimpleType(MemberT* ptr, const Metadata<MemberT, DescriptorT>& metadata);

    template <typename MemberT, typename DescriptorT>
    static void DisplayArray(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);

    template <typename MemberT, typename DescriptorT>
    static void DisplayList(MemberT* obj, const Metadata<MemberT, DescriptorT>& metadata);

    template <typename MemberT, typename DescriptorT>
    [[nodiscard]]
    static constexpr size_t GetFlags(DescriptorT member);


    // TODO maybe move elsewhere
    template <XnorCore::Concepts::ResourceT T>
    static XnorCore::Pointer<T> FilterResources(ImGuiTextFilter& filter);

    static inline XnorCore::Entity* FilterEntity(ImGuiTextFilter& filter);

    static inline void* m_ResourceFilterTarget = nullptr;
    static inline void* m_EntityFilterTarget = nullptr;
    static inline ImGuiTextFilter m_TextFilter;
};

END_XNOR_EDITOR

#include "windows/inspector.inl"
