#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "magic_enum/magic_enum_all.hpp"
#include "rendering/light/directional_light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "scene/component/test_component.hpp"
#include "utils/utils.hpp"

BEGIN_XNOR_EDITOR

template <typename MemberT>
void Inspector::DisplayScalar(MemberT* const obj, const char_t* name)
{
    uint32_t type;

    if constexpr (XnorCore::Meta::IsSame<MemberT, int32_t>)
        type = ImGuiDataType_S32;
    else if constexpr (XnorCore::Meta::IsSame<MemberT, uint32_t>)
        type = ImGuiDataType_U32;
    else if constexpr (XnorCore::Meta::IsSame<MemberT, int16_t>)
        type = ImGuiDataType_S16;
    else if constexpr (XnorCore::Meta::IsSame<MemberT, uint16_t>)
        type = ImGuiDataType_U16;
    else if constexpr (XnorCore::Meta::IsSame<MemberT, int8_t>)
        type = ImGuiDataType_S8;
    else if constexpr (XnorCore::Meta::IsSame<MemberT, uint8_t>)
        type = ImGuiDataType_U8;
    else if constexpr (XnorCore::Meta::IsSame<MemberT, float_t>)
        type = ImGuiDataType_Float;
    else if constexpr (XnorCore::Meta::IsSame<MemberT, double_t>)
        type = ImGuiDataType_Double;

    ImGui::InputScalar(name, type, obj);
}

template <typename MemberT>
void Inspector::DisplayMathType(MemberT* const obj, const char_t* name)
{
    if constexpr (XnorCore::Meta::IsSame<MemberT, Vector2i>)
    {
        ImGui::DragInt2(name, obj->Raw(), 0.1f);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, Vector2>)
    {
        ImGui::DragFloat2(name, obj->Raw(), 0.1f);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, Vector3>)
    {
        ImGui::DragFloat3(name, obj->Raw(), 0.1f);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, Vector4>)
    {
        ImGui::DragFloat4(name, obj->Raw(), 0.1f);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, Quaternion>)
    {
        Vector3 euler = XnorCore::Utils::GetQuaternionEulerAngles(*obj);
        ImGui::SliderAngle3(name, euler.Raw());
        
        *obj = Quaternion::FromEuler(euler);
    }
}

template <typename MemberT>
void Inspector::DisplayColorType(MemberT* obj, const char_t* name)
{
    if constexpr (XnorCore::Meta::IsSame<MemberT, XnorCore::ColorRgb>)
    {
        XnorCore::Colorf tmp = static_cast<XnorCore::Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *obj = static_cast<XnorCore::ColorRgb>(tmp);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, XnorCore::ColorHsv>)
    {
        XnorCore::Colorf tmp = static_cast<XnorCore::Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *obj = static_cast<XnorCore::ColorHsv>(tmp);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, XnorCore::Colorf>)
    {
        ImGui::ColorPicker4(name, reinterpret_cast<float_t*>(obj));
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, XnorCore::ColorRgba>)
    {
        XnorCore::Colorf tmp = static_cast<XnorCore::Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *obj = static_cast<XnorCore::ColorRgba>(tmp);
    }
}

template <typename MemberT>
void Inspector::DisplayXnorPointer(MemberT* obj, const char_t* name)
{
    using PtrT = typename MemberT::Type;

    ImGui::PushID(obj);
    
    ImGui::Text("%s", name);

    if constexpr (XnorCore::Meta::IsBaseOf<XnorCore::Resource, PtrT>)
    {
        ImGui::SameLine();

        if (*obj != nullptr)
            ImGui::Text("%s", obj->Get()->GetName().c_str());
        else
            ImGui::Text("No resource");

        if (ImGui::BeginDragDropTarget())
        {
            // ReSharper disable once CppTooWideScope
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("ContentBrowserFile");
                
            if (payload)
            {
                XnorCore::Pointer<XnorCore::Resource> dragged = *static_cast<XnorCore::Pointer<XnorCore::Resource>*>(payload->Data);

                const XnorCore::Resource* raw = static_cast<XnorCore::Resource*>(dragged);
                if (XnorCore::Utils::GetTypeHash<XnorCore::Resource>(raw) == XnorCore::Utils::GetTypeHash<PtrT>())
                {
                    *reinterpret_cast<decltype(dragged)*>(obj) = dragged;
                }
            }
                
            ImGui::EndDragDropTarget();
        }
        
        ImGui::SameLine();

        if (ImGui::Button("+"))
        {
            m_ResourceFilterTarget = static_cast<void*>(obj);
        }

        if (m_ResourceFilterTarget == static_cast<void*>(obj))
        {
            XnorCore::Pointer<PtrT> res = FilterResources<PtrT>();
            if (res)
            {
                *obj = res;
                m_ResourceFilterTarget = nullptr;
            }
        }
    }

    ImGui::PopID();
}

template <typename MemberT>
void Inspector::DisplayRawPointer(MemberT* obj, const char_t* name)
{
    using TypeT = XnorCore::Meta::RemovePointerSpecifier<MemberT>;

    ImGui::Text("%s", name);

    if constexpr (XnorCore::Meta::IsSame<TypeT, XnorCore::Entity>)
    {
        ImGui::SameLine();

        if (*obj != nullptr)
            ImGui::Selectable((*obj)->name.c_str());
        else
            ImGui::Selectable("nullptr");

        if (ImGui::BeginDragDropTarget())
        {
            // ReSharper disable once CppTooWideScope
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("HierarchyEntity");
        
            if (payload)
            {
                XnorCore::Entity* const dragged = *static_cast<XnorCore::Entity**>(payload->Data);

                *obj = dragged;
            }
            
            ImGui::EndDragDropTarget();
        }
    }
}

#define POLY_PTR_IF_INSP(type)\
if (hash == XnorCore::Utils::GetTypeHash<type>())\
{\
DisplayObject<type>(obj->Cast<type>(), XnorCore::Reflection::GetTypeInfo<type>());\
}\

template <typename MemberT>
void Inspector::DisplayPolyPointer(MemberT* obj, const char_t* name)
{
    const size_t hash = obj->GetHash();

    if (ImGui::CollapsingHeader(name))
    {
        // TODO find a less ugly solution to that

        POLY_PTR_IF_INSP(XnorCore::MeshRenderer)
        POLY_PTR_IF_INSP(XnorCore::DirectionalLight)
        POLY_PTR_IF_INSP(XnorCore::TestComponent)
        POLY_PTR_IF_INSP(XnorCore::PointLight)
        POLY_PTR_IF_INSP(XnorCore::SpotLight)
    }
}

template <typename MemberT>
void Inspector::DisplayEnum(MemberT* obj, const char_t* name)
{
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    using NamesArrayT = decltype(enumNames);

    constexpr auto getter = [](void* const userData, const int32_t idx) -> const char_t*
    {
        const auto ptr = static_cast<NamesArrayT*>(userData);
        return ptr->at(idx).data();
    };

    ImGui::Combo(name, reinterpret_cast<int32_t*>(obj), getter, reinterpret_cast<void*>(const_cast<XnorCore::Meta::RemoveConstSpecifier<NamesArrayT>*>(&enumNames)), static_cast<int32_t>(enumNames.size()));
}

template <typename ReflectT>
void Inspector::DisplayObject(ReflectT* const obj, const XnorCore::TypeDescriptor<ReflectT> desc)
{
    constexpr const char_t* const typeName = desc.name.c_str();
    const float_t textSize = ImGui::CalcTextSize(typeName).x;
    XnorCore::Utils::AlignImGuiCursor(textSize);
    ImGui::Text("%s", typeName);

    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        using MemberT = typename T::value_type;
        using NotifyChangeT = XnorCore::NotifyChange<ReflectT>;
        
        constexpr bool_t hidden = XnorCore::Reflection::HasAttribute<XnorCore::HideInInspector>(member);
        constexpr bool_t notifyChange = XnorCore::Reflection::HasAttribute<NotifyChangeT>(member);

        if constexpr (!hidden)
        {
            if constexpr (notifyChange)
            {
                MemberT oldValue = member.get(obj);

                DisplayObjectInternal<ReflectT, MemberT, T>(obj, member);
                
                MemberT newValue = member.get(obj);

                if (newValue != oldValue)
                {
                    const auto notify = XnorCore::Reflection::GetAttribute<NotifyChangeT>(member);
                    obj->*notify.pointer = true;
                }
            }
            else
            {
                DisplayObjectInternal<ReflectT, MemberT, T>(obj, member);
            }
        }
    });
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Inspector::DisplayObjectInternal(ReflectT* obj, DescriptorT member)
{
    const constexpr char_t* const name = member.name.c_str();
            
    if constexpr (XnorCore::Meta::IsArray<MemberT>)
    {
        DisplayArray<MemberT>(&member.get(obj), name);
    }
    else if constexpr (XnorCore::Meta::IsXnorList<MemberT>)
    {
        DisplayList<MemberT>(&member.get(obj), name);
    }
    else
    {
        DisplaySimpleType<MemberT>(&member.get(obj), name);
    }
}

template <typename MemberT>
void Inspector::DisplaySimpleType(MemberT* ptr, const char_t* name)
{
    if constexpr (XnorCore::Meta::IsIntegralOrFloating<MemberT>)
    {
        DisplayScalar<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsMathType<MemberT>)
    {
        DisplayMathType<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsColorType<MemberT>)
    {
        DisplayColorType<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, bool_t>)
    {
        ImGui::Checkbox(name, ptr);
    }
    else if constexpr (XnorCore::Meta::IsSame<MemberT, std::string>)
    {
        ImGui::InputText(name, ptr);
    }
    else if constexpr (XnorCore::Meta::IsPointer<MemberT>)
    {
        DisplayRawPointer<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsPolyPtr<MemberT>)
    {
        DisplayPolyPointer<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsXnorPointer<MemberT>)
    {
        DisplayXnorPointer<MemberT>(ptr, name);
    }
    else if constexpr (XnorCore::Meta::IsEnum<MemberT>)
    {
        DisplayEnum<MemberT>(ptr, name);
    }
    else
    {
        if (ImGui::CollapsingHeader(name))
            DisplayObject<MemberT>(ptr, XnorCore::Reflection::GetTypeInfo<MemberT>());
    }
}

template <typename MemberT>
void Inspector::DisplayArray(MemberT* ptr, const char_t* name)
{
    using ArrayT = XnorCore::Meta::RemoveArraySpecifier<MemberT>;
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
        if constexpr (!XnorCore::Meta::IsPolyPtr<ArrayT>)
        {
            if (ImGui::Selectable("Add"))
            {
                ptr->Add();
            }
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

template <XnorCore::ResourceT T>
XnorCore::Pointer<T> Inspector::FilterResources()
{
    ImGui::OpenPopup("Resource");
    
    if (!ImGui::BeginPopupModal("Resource"))
        return nullptr;

    XnorCore::Pointer<T> r = nullptr;
    std::vector<XnorCore::Pointer<T>> resources = XnorCore::ResourceManager::FindAll<T>();

    for (const XnorCore::Pointer<T>& res : resources)
    {
        if (ImGui::Selectable(res.Get()->GetName().c_str()))
        {
            r = res;
            break;
        }
    }
    
    ImGui::EndPopup();
    return r;
}

END_XNOR_EDITOR
