#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "magic_enum/magic_enum_all.hpp"
#include "utils/utils.hpp"
#include "world/world.hpp"

BEGIN_XNOR_CORE

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayScalar(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    uint32_t type;

    if constexpr (Meta::IsSame<MemberT, int32_t>)
        type = ImGuiDataType_S32;
    else if constexpr (Meta::IsSame<MemberT, uint32_t>)
        type = ImGuiDataType_U32;
    else if constexpr (Meta::IsSame<MemberT, int16_t>)
        type = ImGuiDataType_S16;
    else if constexpr (Meta::IsSame<MemberT, uint16_t>)
        type = ImGuiDataType_U16;
    else if constexpr (Meta::IsSame<MemberT, int8_t>)
        type = ImGuiDataType_S8;
    else if constexpr (Meta::IsSame<MemberT, uint8_t>)
        type = ImGuiDataType_U8;
    else if constexpr (Meta::IsSame<MemberT, float_t>)
        type = ImGuiDataType_Float;
    else if constexpr (Meta::IsSame<MemberT, double_t>)
        type = ImGuiDataType_Double;

    if (metadata.range)
    {
        ImGui::SliderScalar(metadata.name, type, metadata.obj, &metadata.range->minimum, &metadata.range->maximum);        
    }
    else
    {
        ImGui::DragScalar(metadata.name, type, metadata.obj, .1f);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayMathType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Meta::IsSame<MemberT, Vector2i>)
    {
        ImGui::DragInt2(metadata.name, metadata.obj->Raw(), 0.1f);
    }
    else if constexpr (Meta::IsSame<MemberT, Vector2>)
    {
        ImGui::DragFloat2(metadata.name, metadata.obj->Raw(), 0.1f);
    }
    else if constexpr (Meta::IsSame<MemberT, Vector3>)
    {
        ImGui::DragFloat3(metadata.name, metadata.obj->Raw(), 0.1f);
    }
    else if constexpr (Meta::IsSame<MemberT, Vector4>)
    {
        ImGui::DragFloat4(metadata.name, metadata.obj->Raw(), 0.1f);
    }
    else if constexpr (Meta::IsSame<MemberT, Quaternion>)
    {
        Vector3 euler = Utils::GetQuaternionEulerAngles(*metadata.obj);
        ImGui::SliderAngle3(metadata.name, euler.Raw());
        
        *metadata.obj = Quaternion::FromEuler(euler);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayColorType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Meta::IsSame<MemberT, ColorRgb>)
    {
        Colorf tmp = static_cast<Colorf>(*metadata.obj);
        ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *metadata.obj = static_cast<ColorRgb>(tmp);
    }
    else if constexpr (Meta::IsSame<MemberT, ColorHsva>)
    {
        Colorf tmp = static_cast<Colorf>(*metadata.obj);
        ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *metadata.obj = static_cast<ColorHsva>(tmp);
    }
    else if constexpr (Meta::IsSame<MemberT, Colorf>)
    {
        ImGui::ColorPicker4(metadata.name, reinterpret_cast<float_t*>(metadata.obj));
    }
    else if constexpr (Meta::IsSame<MemberT, ColorRgba>)
    {
        Colorf tmp = static_cast<Colorf>(*metadata.obj);
        ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *metadata.obj = static_cast<ColorRgba>(tmp);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayXnorPointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using PtrT = typename MemberT::Type;

    ImGui::PushID(metadata.obj);
    
    ImGui::Text("%s", metadata.name);

    if constexpr (Meta::IsBaseOf<Resource, PtrT>)
    {
        ImGui::SameLine();

        if (*metadata.obj != nullptr)
        {
            const std::string resName = metadata.obj->Get()->GetName();
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
                *metadata.obj = nullptr;

            ImGui::EndPopup();
        }

        if (ImGui::BeginDragDropTarget())
        {
            // ReSharper disable once CppTooWideScope
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("ContentBrowserFile");
                
            if (payload)
            {
                Pointer<Resource> dragged = *static_cast<Pointer<Resource>*>(payload->Data);

                const Resource* raw = static_cast<Resource*>(dragged);
                if (Utils::GetTypeHash<Resource>(raw) == Utils::GetTypeHash<PtrT>())
                {
                    *reinterpret_cast<decltype(dragged)*>(metadata.obj) = dragged;
                }
            }
                
            ImGui::EndDragDropTarget();
        }
        
        ImGui::SameLine();

        if (ImGui::Button("+"))
        {
            m_ResourceFilterTarget = static_cast<void*>(metadata.obj);
            m_TextFilter.Clear();
        }

        if (m_ResourceFilterTarget == static_cast<void*>(metadata.obj))
        {
            Pointer<PtrT> res = FilterResources<PtrT>(m_TextFilter);
            if (res)
            {
                *metadata.obj = res;
                m_ResourceFilterTarget = nullptr;
            }
        }
    }

    ImGui::PopID();
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayRawPointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using TypeT = Meta::RemovePointerSpecifier<MemberT>;

    if constexpr (Meta::IsSame<TypeT, Entity>)
    {
        ImGui::Text("%s", metadata.name);
        ImGui::SameLine();

        if (*metadata.obj != nullptr)
            ImGui::Text("%s", (*metadata.obj)->name.c_str());
        else
            ImGui::Text("nullptr");

        if (ImGui::BeginDragDropTarget())
        {
            // ReSharper disable once CppTooWideScope
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("HierarchyEntity");
        
            if (payload)
            {
                Entity* const dragged = *static_cast<Entity**>(payload->Data);

                *metadata.obj = dragged;
            }
            
            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();

        if (ImGui::Button("+"))
        {
            m_EntityFilterTarget = static_cast<void*>(metadata.obj);
            m_TextFilter.Clear();
        }

        if (m_EntityFilterTarget == static_cast<void*>(metadata.obj))
        {
            Entity* e = FilterEntity(m_TextFilter);
            if (e)
            {
                *metadata.obj = e;
                m_EntityFilterTarget = nullptr;
            }
        }
    }
    else if constexpr (Meta::IsSame<TypeT, Component>)
    {
        const size_t hash = Utils::GetTypeHash<Component>(*metadata.obj);

        if (ImGui::CollapsingHeader(metadata.name))
        {
            DisplayObjectUsingFactory(*metadata.obj, hash);
        }
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayEnum(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    using NamesArrayT = decltype(enumNames);

    constexpr auto getter = [](void* const userData, const int32_t idx) -> const char_t*
    {
        const auto ptr = static_cast<NamesArrayT*>(userData);
        return ptr->at(idx).data();
    };

    ImGui::Combo(metadata.name, reinterpret_cast<int32_t*>(metadata.obj), getter, reinterpret_cast<void*>(const_cast<Meta::RemoveConstSpecifier<NamesArrayT>*>(&enumNames)), static_cast<int32_t>(enumNames.size()));
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayEnumFlag(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    constexpr size_t size = enumNames.size();

    std::string previewValue;
    for (size_t i = 0; i < size; i++)
    {
        const size_t value = static_cast<size_t>(*metadata.obj);
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

    if (ImGui::BeginCombo(metadata.name, previewValue.c_str()))
    {
        for (size_t i = 0; i < size; i++)
        {
            const size_t enumValue = static_cast<size_t>(magic_enum::enum_value<MemberT>(i));
            size_t value = static_cast<size_t>(*metadata.obj);
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

                *metadata.obj = static_cast<MemberT>(value);
            }
        }
        
        ImGui::EndCombo();
    }
}

template <typename ReflectT>
void TypeRenderer::DisplayObject(ReflectT* const obj)
{
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();
    const std::string typeName = Utils::RemoveNamespaces(desc.name.c_str());
    const float_t textSize = ImGui::CalcTextSize(typeName.c_str()).x;
    Utils::AlignImGuiCursor(textSize);
    ImGui::Text("%s", typeName.c_str());

    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        using MemberT = typename T::value_type;
        using NotifyChangeT = Reflection::NotifyChange<ReflectT>;
        
        constexpr bool_t hidden = Reflection::HasAttribute<Reflection::HideInInspector>(member);
        constexpr bool_t notifyChange = Reflection::HasAttribute<NotifyChangeT>(member);

        if constexpr (!hidden)
        {
            if constexpr (notifyChange)
            {
                MemberT oldValue = member.get(obj);

                DisplayObjectInternal<ReflectT, MemberT, T>(obj, member);
                
                MemberT newValue = member.get(obj);

                if (newValue != oldValue)
                {
                    const auto notify = Reflection::GetAttribute<NotifyChangeT>(member);
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
void TypeRenderer::DisplayObjectInternal(ReflectT* obj, DescriptorT member)
{
    const std::string n = Utils::HumanizeVariableName(member.name.c_str());
    const char_t* const name = n.c_str();

    const Metadata<ReflectT, MemberT, DescriptorT> metadata = {
        .topLevelObj = obj,
        .name = name,
        .obj = &member.get(obj),
        .descriptor = member,
        .range = Reflection::TryGetAttribute<Reflection::Range<MemberT>, DescriptorT>(member),
        .tooltip = Reflection::TryGetAttribute<Reflection::Tooltip, DescriptorT>(member)
    };

    if constexpr (Meta::IsArray<MemberT>)
    {
        DisplayArray<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsXnorList<MemberT>)
    {
        DisplayList<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else
    {
        DisplaySimpleType<ReflectT, MemberT, DescriptorT>(metadata);
    }

    if (metadata.tooltip)
        ImGui::SetItemTooltip("%s", metadata.tooltip->text);
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplaySimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Meta::IsIntegralOrFloating<MemberT>)
    {
        DisplayScalar<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsMathType<MemberT>)
    {
        DisplayMathType<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsColorType<MemberT>)
    {
        DisplayColorType<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsSame<MemberT, bool_t>)
    {
        ImGui::Checkbox(metadata.name, metadata.obj);
    }
    else if constexpr (Meta::IsSame<MemberT, std::string>)
    {
        ImGui::InputText(metadata.name, metadata.obj);
    }
    else if constexpr (Meta::IsPointer<MemberT>)
    {
        DisplayRawPointer<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsXnorPointer<MemberT>)
    {
        DisplayXnorPointer<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsEnum<MemberT>)
    {
        if (metadata.template HasAttribute<Reflection::EnumFlags>())
            DisplayEnumFlag<ReflectT, MemberT, DescriptorT>(metadata);
        else
            DisplayEnum<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else
    {
        if (ImGui::CollapsingHeader(metadata.name))
            DisplayObject<MemberT>(metadata.obj);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayArray(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using ArrayT = Meta::RemoveArraySpecifier<MemberT>;
    constexpr size_t arraySize = sizeof(MemberT) / sizeof(ArrayT);

    if (ImGui::CollapsingHeader(metadata.name))
    {
        Metadata<ReflectT, ArrayT, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = "",
            .descriptor = metadata.descriptor,
            .range = Reflection::TryGetAttribute<Reflection::Range<ArrayT>>(metadata.descriptor),
            .tooltip = metadata.tooltip
        };
        
        for (size_t i = 0; i < arraySize; i++)
        {
            ImGui::PushID(&i + i);
            
            const std::string index = std::to_string(i);

            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];
            
            DisplaySimpleType<ReflectT, ArrayT, DescriptorT>(metadataArray);

            ImGui::PopID();
        }
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayList(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using ArrayT = typename MemberT::Type;
    constexpr bool_t isComponentList = Meta::IsSame<ArrayT, Component*>;

    if (ImGui::CollapsingHeader(metadata.name))
    {
        if (ImGui::Selectable("Add"))
        {
            if constexpr (isComponentList)
            {
                m_ComponentFilterTarget = static_cast<void*>(metadata.obj);
                m_TextFilter.Clear();         
            }
            else
            {
                metadata.obj->Add();
            }
        }

        Metadata<ReflectT, ArrayT, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = "",
            .descriptor = metadata.descriptor,
            .range = Reflection::TryGetAttribute<Reflection::Range<ArrayT>>(metadata.descriptor),
            .tooltip = metadata.tooltip
        };

        size_t listSize = metadata.obj->GetSize();
                
        for (size_t i = 0; i < listSize; i++)
        {
            ImGui::PushID(&i + i);
            if (ImGui::Button("-"))
            {
                metadata.obj->RemoveAt(i);
                --listSize;

                if (listSize == 0 || i == listSize)
                {
                    ImGui::PopID();
                    break;
                }
            }
                
            ImGui::SameLine();
                    
            if constexpr (!isComponentList)
            {
                if (ImGui::Button("+"))
                {
                    metadata.obj->Insert(i);
                    ++listSize;
                }
            }
                
            ImGui::SameLine();

            const std::string index = std::to_string(i);
            
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            DisplaySimpleType<ReflectT, ArrayT, DescriptorT>(metadataArray);

            ImGui::PopID();
        }

        if constexpr (isComponentList)
        {
            if (m_ComponentFilterTarget == static_cast<void*>(metadata.obj))
            {
                Component* const e = FilterComponent(m_TextFilter);
                if (e)
                {
                    metadata.obj->Add(e);
                    e->entity = metadata.topLevelObj;
                    m_ComponentFilterTarget = nullptr;
                }
            }
        }
    }
}

template <Concepts::ResourceT T>
Pointer<T> TypeRenderer::FilterResources(ImGuiTextFilter& filter)
{
    ImGui::OpenPopup("Resource");

    if (!ImGui::BeginPopupModal("Resource"))
        return nullptr;

    filter.Draw();
    std::vector<Pointer<T>> resources = ResourceManager::FindAll<T>(
        [&](Pointer<T> r) -> bool_t
        {
            const std::string& name = r->GetName();

            if (name.starts_with("assets_internal"))
                return false;

            return filter.PassFilter(name.c_str());
        }
    );

    Pointer<T> r = nullptr;
    for (const Pointer<T>& res : resources)
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

inline Entity* TypeRenderer::FilterEntity(ImGuiTextFilter& filter)
{
    ImGui::OpenPopup("Entity");

    if (!ImGui::BeginPopupModal("Entity"))
        return nullptr;

    filter.Draw();
    const List<Entity*>& entities = World::scene.GetEntities();

    Entity* e = nullptr;
    for (size_t i = 0; i < entities.GetSize(); i++)
    {
        const char_t* const name = entities[i]->name.c_str();
        if (filter.PassFilter(name) && ImGui::Selectable(name))
        {
            e = entities[i];
            break;
        }
    }
    
    ImGui::EndPopup();
    return e;
}

END_XNOR_CORE
