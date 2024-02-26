#pragma once

#include "definitions.hpp"
#include "ui_window.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "utils/reflectable.hpp"
#include "utils/utils.hpp"

BEGIN_XNOR_EDITOR

class Inspector : public UiWindow
{
public:
    using UiWindow::UiWindow;

    explicit Inspector(Editor* editor);
    
    void Display() override;

private:
    void DisplayMember(void* obj, const XnorCore::FieldInfo& fieldInfo);
    void DisplayScalarMember(void* obj, const XnorCore::FieldInfo& fieldInfo, size_t element);
    void DisplayArrayMember(void* obj, const XnorCore::FieldInfo& fieldInfo);
    void DisplayVectorMember(void* obj, const XnorCore::FieldInfo& fieldInfo);
    void DisplayXnorPointerMember(const void* obj, const XnorCore::FieldInfo& fieldInfo);

    void DisplayNativeType(void* obj, const XnorCore::FieldInfo& fieldInfo, const char_t* name, size_t element);
    void DisplayMathType(void* obj, const XnorCore::FieldInfo& fieldInfo, const char_t* name, size_t element);
    void DisplayNestedType(void* obj, const XnorCore::FieldInfo& fieldInfo, const char_t* name, size_t element);

    template <typename T>
    static void DisplayScalar(void* obj, size_t offset, const char_t* name, size_t element);

    template <typename MemberT>
    static void DisplayScalarUsingDescriptor(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayMathTypeUsingDescriptor(MemberT* obj, const char_t* name);
    template <typename MemberT>
    static void DisplayColorTypeUsingDescriptor(MemberT* obj, const char_t* name);

    template <typename ReflectT>
    static void DisplayUsingDescriptor(ReflectT* obj, XnorCore::TypeDescriptor<ReflectT> desc);

    template <typename MemberT>
    static void DisplaySimpleType(MemberT* ptr, const char_t* name);

    template <typename MemberT>
    static void DisplayArray(MemberT* ptr, const char_t* name);

    template <typename MemberT>
    static void DisplayList(MemberT* ptr, const char_t* name);
};

template <typename T>
void Inspector::DisplayScalar(void* const obj, const size_t offset, const char_t* const name, const size_t element)
{
    uint32_t type;

    if constexpr (std::is_same_v<T, int32_t>)
        type = ImGuiDataType_S32;
    else if constexpr (std::is_same_v<T, uint32_t>)
        type = ImGuiDataType_U32;
    else if constexpr (std::is_same_v<T, int16_t>)
        type = ImGuiDataType_S16;
    else if constexpr (std::is_same_v<T, uint16_t>)
        type = ImGuiDataType_U16;
    else if constexpr (std::is_same_v<T, int8_t>)
        type = ImGuiDataType_S8;
    else if constexpr (std::is_same_v<T, uint8_t>)
        type = ImGuiDataType_U8;
    else if constexpr (std::is_same_v<T, float_t>)
        type = ImGuiDataType_Float;
    else if constexpr (std::is_same_v<T, double_t>)
        type = ImGuiDataType_Double;

    ImGui::InputScalar(name, type, XnorCore::Utils::GetAddress<T>(obj, offset, element));
}

template <typename MemberT>
void Inspector::DisplayScalarUsingDescriptor(MemberT* const obj, const char_t* name)
{
    uint32_t type;

    if constexpr (std::is_same_v<MemberT, int32_t>)
        type = ImGuiDataType_S32;
    else if constexpr (std::is_same_v<MemberT, uint32_t>)
        type = ImGuiDataType_U32;
    else if constexpr (std::is_same_v<MemberT, int16_t>)
        type = ImGuiDataType_S16;
    else if constexpr (std::is_same_v<MemberT, uint16_t>)
        type = ImGuiDataType_U16;
    else if constexpr (std::is_same_v<MemberT, int8_t>)
        type = ImGuiDataType_S8;
    else if constexpr (std::is_same_v<MemberT, uint8_t>)
        type = ImGuiDataType_U8;
    else if constexpr (std::is_same_v<MemberT, float_t>)
        type = ImGuiDataType_Float;
    else if constexpr (std::is_same_v<MemberT, double_t>)
        type = ImGuiDataType_Double;

    ImGui::InputScalar(name, type, obj);
}

template <typename MemberT>
void Inspector::DisplayMathTypeUsingDescriptor(MemberT* const obj, const char_t* name)
{
    if constexpr (std::is_same_v<MemberT, Vector2i>)
    {
        ImGui::DragInt2(name, obj->Raw(), 0.1f);
    }
    else if constexpr (std::is_same_v<MemberT, Vector2>)
    {
        ImGui::DragFloat2(name, obj->Raw(), 0.1f);
    }
    else if constexpr (std::is_same_v<MemberT, Vector3>)
    {
        ImGui::DragFloat3(name, obj->Raw(), 0.1f);
    }
    else if constexpr (std::is_same_v<MemberT, Vector4>)
    {
        ImGui::DragFloat4(name, obj->Raw(), 0.1f);
    }
    else if constexpr (std::is_same_v<MemberT, Quaternion>)
    {
        Vector3 euler = XnorCore::Utils::GetQuaternionEulerAngles(*obj);
        ImGui::SliderAngle3(name, euler.Raw());
        
        *obj = Quaternion::FromEuler(euler);
    }
}

template <typename MemberT>
void Inspector::DisplayColorTypeUsingDescriptor(MemberT* obj, const char_t* name)
{
    if constexpr (std::is_same_v<MemberT, XnorCore::ColorRgb>)
    {
        XnorCore::Colorf tmp = static_cast<XnorCore::Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *obj = static_cast<XnorCore::ColorRgb>(tmp);
    }
    else if constexpr (std::is_same_v<MemberT, XnorCore::ColorHsv>)
    {
        XnorCore::Colorf tmp = static_cast<XnorCore::Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *obj = static_cast<XnorCore::ColorHsv>(tmp);
    }
    else if constexpr (std::is_same_v<MemberT, XnorCore::Colorf>)
    {
        ImGui::ColorPicker4(name, reinterpret_cast<float_t*>(obj));
    }
}

template <typename ReflectT>
void Inspector::DisplayUsingDescriptor(ReflectT* const obj, const XnorCore::TypeDescriptor<ReflectT> desc)
{
    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        constexpr bool hidden = refl::descriptor::has_attribute<XnorCore::HideInInspector>(member); 
        
        if constexpr (!hidden)
        {
            using MemberT = typename T::value_type;
            const constexpr char_t* const name = member.name.c_str();
            
            if constexpr (std::is_array_v<MemberT>)
            {
                DisplayArray<MemberT>(&member.get(obj), name);
            }
            else if constexpr (XnorCore::Meta::IsXnorVector<MemberT>)
            {
                DisplayList<MemberT>(&member.get(obj), name);
            }
            else
            {
                DisplaySimpleType<MemberT>(&member.get(obj), name);
            }
        }
    });
}

template <typename MemberT>
void Inspector::DisplaySimpleType(MemberT* ptr, const char_t* name)
{
    if constexpr (XnorCore::Meta::IsIntegralOrFloating<MemberT>)
    {
        DisplayScalarUsingDescriptor<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsMathType<MemberT>)
    {
        DisplayMathTypeUsingDescriptor<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsColorType<MemberT>)
    {
        DisplayColorUsingDescriptor<MemberT>(ptr, name);
    }
    else if constexpr (std::is_same_v<MemberT, bool_t>)
    {
        ImGui::Checkbox(name, ptr);
    }
    else if constexpr (std::is_same_v<MemberT, std::string>)
    {
        ImGui::InputText(name, ptr);
    }
    else if constexpr (XnorCore::Meta::IsPolyPtr<MemberT>)
    {
        [[maybe_unused]] const size_t hash = ptr->GetHash(); 
    }
    else
    {
        if (ImGui::CollapsingHeader(name))
            DisplayUsingDescriptor<MemberT>(ptr, XnorCore::TypeInfo::Get<MemberT>());            
    }
}

template <typename MemberT>
void Inspector::DisplayArray(MemberT* ptr, const char_t* name)
{
    using ArrayT = std::remove_extent_t<MemberT>;
    constexpr size_t arraySize = sizeof(MemberT) / sizeof(ArrayT);

    if (ImGui::CollapsingHeader(name))
    {
        for (size_t i = 0; i < arraySize; i++)
        {
            DisplaySimpleType<ArrayT>(&(*ptr)[i], std::to_string(i).c_str());
        }
    }
}

template <typename MemberT>
void Inspector::DisplayList(MemberT* ptr, const char_t* name)
{
    using ArrayT = typename MemberT::Type;

    if (ImGui::CollapsingHeader(name))
    {
        if (ImGui::Selectable("Add"))
        {
            ptr->Add();
        }
            
        size_t listSize = ptr->GetSize();
                
        for (size_t i = 0; i < listSize; i++)
        {
            ImGui::PushID(static_cast<int32_t>(i));
            if (ImGui::Button("-"))
            {
                ptr->RemoveAt(i);
                --listSize;

                if (listSize == 0 || i == listSize)
                {
                    ImGui::PopID();
                    break;
                }
            }
                
            ImGui::SameLine();
                    
            if constexpr (!XnorCore::Meta::IsPolyPtr<ArrayT>)
            {
                if (ImGui::Button("+"))
                {
                    ptr->Insert(i);
                    ++listSize;
                }
            }
                
            ImGui::SameLine();
                    
            DisplaySimpleType<ArrayT>(&(*ptr)[i], std::to_string(i).c_str());
            ImGui::PopID();
        }
    }
}

END_XNOR_EDITOR
