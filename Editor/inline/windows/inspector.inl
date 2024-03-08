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
void Inspector::DisplayScalar(MemberT* const obj, const char_t* name, [[maybe_unused]] const size_t flags)
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
void Inspector::DisplayMathType(MemberT* const obj, const char_t* name, [[maybe_unused]] const size_t flags)
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
void Inspector::DisplayColorType(MemberT* obj, const char_t* name, [[maybe_unused]] const size_t flags)
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
void Inspector::DisplayXnorPointer(MemberT* obj, const char_t* name, [[maybe_unused]] const size_t flags)
{
    using PtrT = typename MemberT::Type;

    ImGui::PushID(obj);
    
    ImGui::Text("%s", name);

    if constexpr (XnorCore::Meta::IsBaseOf<XnorCore::Resource, PtrT>)
    {
        ImGui::SameLine();

        if (*obj != nullptr)
        {
            const std::string resName = obj->Get()->GetName();
            // TODO clamp resource name
            const float_t textSize = std::clamp<float_t>(ImGui::CalcTextSize(resName.c_str()).x, 0.f, 5.f);
            ImGui::SetNextItemWidth(textSize);
            ImGui::Text("%s", resName.c_str());
        }
        else
        {
            ImGui::Text("No resource");
        }

        if (ImGui::BeginPopupContextItem("RemoveResPopUp"))
        {
            if (ImGui::Selectable("Remove"))
                *obj = nullptr;

            ImGui::EndPopup();
        }

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
            m_ResourceFilterName.Clear();
        }

        if (m_ResourceFilterTarget == static_cast<void*>(obj))
        {
            XnorCore::Pointer<PtrT> res = FilterResources<PtrT>(m_ResourceFilterName);
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
void Inspector::DisplayRawPointer(MemberT* obj, const char_t* name, [[maybe_unused]] const size_t flags)
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
void Inspector::DisplayPolyPointer(MemberT* obj, const char_t* name, [[maybe_unused]] const size_t flags)
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
void Inspector::DisplayEnum(MemberT* obj, const char_t* name, [[maybe_unused]] const size_t flags)
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

template <typename MemberT>
void Inspector::DisplayEnumFlag(MemberT* obj, const char_t* name, [[maybe_unused]] size_t flags)
{
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    constexpr size_t size = enumNames.size();

    std::string previewValue;
    for (size_t i = 0; i < size; i++)
    {
        const size_t value = static_cast<size_t>(*obj);
        const MemberT enumValue = magic_enum::enum_value<MemberT>(i);

        if (value & static_cast<size_t>(enumValue))
        {
            if (previewValue.empty())
            {
                previewValue = magic_enum::enum_name<MemberT>(enumValue).data();
            }
            else
            {
                previewValue += ", ";
                previewValue += magic_enum::enum_name<MemberT>(enumValue).data();
            }
        }
    }

    if (ImGui::BeginCombo(name, previewValue.c_str()))
    {
        for (size_t i = 0; i < size; i++)
        {
            const size_t enumValue = static_cast<size_t>(magic_enum::enum_value<MemberT>(i));
            size_t value = static_cast<size_t>(*obj);
            const bool_t isSelected = (value & enumValue) != 0;
            bool_t tmpSelected = isSelected;

            if (ImGui::MenuItem(enumNames.at(i).data(), nullptr, &tmpSelected))
            {
                if (enumValue == 0)
                {
                    value = 0;
                }
                else
                {
                    if (isSelected)
                        value &= ~enumValue;
                    else
                        value |= enumValue;
                }

                *obj = static_cast<MemberT>(value);
            }
        }
        
        ImGui::EndCombo();
    }
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
    constexpr size_t flags = GetFlags<MemberT, DescriptorT>(member);
    MemberT* const ptr = &member.get(obj);
            
    if constexpr (XnorCore::Meta::IsArray<MemberT>)
    {
        DisplayArray<MemberT>(ptr, name, flags);
    }
    else if constexpr (XnorCore::Meta::IsXnorList<MemberT>)
    {
        DisplayList<MemberT>(ptr, name, flags);
    }
    else
    {
        DisplaySimpleType<MemberT>(ptr, name, flags);
    }
}

template <typename MemberT>
void Inspector::DisplaySimpleType(MemberT* ptr, const char_t* name, [[maybe_unused]] const size_t flags)
{
    if constexpr (XnorCore::Meta::IsIntegralOrFloating<MemberT>)
    {
        DisplayScalar<MemberT>(ptr, name, flags);
    }
    else if constexpr (XnorCore::Meta::IsMathType<MemberT>)
    {
        DisplayMathType<MemberT>(ptr, name, flags);
    }
    else if constexpr (XnorCore::Meta::IsColorType<MemberT>)
    {
        DisplayColorType<MemberT>(ptr, name, flags);
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
        DisplayRawPointer<MemberT>(ptr, name, flags);
    }
    else if constexpr (XnorCore::Meta::IsPolyPtr<MemberT>)
    {
        DisplayPolyPointer<MemberT>(ptr, name, flags);
    }
    else if constexpr (XnorCore::Meta::IsXnorPointer<MemberT>)
    {
        DisplayXnorPointer<MemberT>(ptr, name, flags);
    }
    else if constexpr (XnorCore::Meta::IsEnum<MemberT>)
    {
        if (flags & InspectorFlags::EnumFlag)
            DisplayEnumFlag<MemberT>(ptr, name, flags);
        else
            DisplayEnum<MemberT>(ptr, name, flags);
    }
    else
    {
        if (ImGui::CollapsingHeader(name))
            DisplayObject<MemberT>(ptr, XnorCore::Reflection::GetTypeInfo<MemberT>());
    }
}

template <typename MemberT>
void Inspector::DisplayArray(MemberT* ptr, const char_t* name, [[maybe_unused]] const size_t flags)
{
    using ArrayT = XnorCore::Meta::RemoveArraySpecifier<MemberT>;
    constexpr size_t arraySize = sizeof(MemberT) / sizeof(ArrayT);

    if (ImGui::CollapsingHeader(name))
    {
        for (size_t i = 0; i < arraySize; i++)
        {
            DisplaySimpleType<ArrayT>(&(*ptr)[i], std::to_string(i).c_str(), flags);
        }
    }
}

template <typename MemberT>
void Inspector::DisplayList(MemberT* ptr, const char_t* name, [[maybe_unused]] const size_t flags)
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

            DisplaySimpleType<ArrayT>(&(*ptr)[i], std::to_string(i).c_str(), flags);

            ImGui::PopID();
        }
    }
}

template <typename MemberT, typename DescriptorT>
constexpr size_t Inspector::GetFlags(DescriptorT member)
{
    size_t flags = InspectorFlags::None;

    if constexpr (XnorCore::Meta::IsEnum<MemberT> && XnorCore::Reflection::HasAttribute<XnorCore::EnumFlags>(member))
        flags |= InspectorFlags::EnumFlag;

    return flags;
}


template <XnorCore::ResourceT T>
XnorCore::Pointer<T> Inspector::FilterResources(ImGuiTextFilter& filter)
{
    ImGui::OpenPopup("Resource");

    if (!ImGui::BeginPopupModal("Resource"))
        return nullptr;

    filter.Draw();
    std::vector<XnorCore::Pointer<T>> resources = XnorCore::ResourceManager::FindAll<T>(
        [&](XnorCore::Pointer<T> r) -> bool_t
        {
            const std::string& name = r->GetName();

            if (name.starts_with("assets_internal"))
                return false;

            return filter.PassFilter(name.c_str());
        }
    );

    XnorCore::Pointer<T> r = nullptr;
    for (const XnorCore::Pointer<T>& res : resources)
    {
        if (ImGui::Selectable(res->GetName().c_str()))
        {
            r = res;
            break;
        }
    }
    
    ImGui::EndPopup();
    return r;
}

END_XNOR_EDITOR
