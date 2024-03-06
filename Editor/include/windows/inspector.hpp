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
    template <typename MemberT>
    static void DisplayScalar(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayMathType(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayColorType(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayRawPointer(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayXnorPointer(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayPolyPointer(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayEnum(MemberT* obj, const char_t* name);

    template <typename ReflectT>
    static void DisplayObject(ReflectT* obj, XnorCore::TypeDescriptor<ReflectT> desc);

    template <typename ReflectT, typename MemberT, typename DescriptorT>
    static void DisplayObjectInternal(ReflectT* obj, DescriptorT member);

    template <typename MemberT>
    static void DisplaySimpleType(MemberT* ptr, const char_t* name);

    template <typename MemberT>
    static void DisplayArray(MemberT* ptr, const char_t* name);

    template <typename MemberT>
    static void DisplayList(MemberT* ptr, const char_t* name);

    // TODO maybe move elsewhere
    template <XnorCore::ResourceT T>
    static XnorCore::Pointer<T> FilterResources();

    static inline void* m_ResourceFilterTarget = nullptr;
};

END_XNOR_EDITOR

#include "windows/inspector.inl"
