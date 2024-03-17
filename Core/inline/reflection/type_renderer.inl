// ReSharper disable CppTooWideScope
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

    // Get imgui data type based on the member type
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
        // Has a range attribute, display as a slider
        ImGui::SliderScalar(metadata.name, type, metadata.obj, &metadata.range->minimum, &metadata.range->maximum);        
    }
    else
    {
        // No range attribute, display as a scalar
        ImGui::DragScalar(metadata.name, type, metadata.obj, .1f);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayMathType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Display vector types as drag
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
        if (metadata.template HasAttribute<Reflection::AsEulerAngles>())
        {
            // Supposed to be euler angles, so draw as a slider angle
            ImGui::SliderAngle3(metadata.name, metadata.obj->Raw());
        }
        else
        {
            ImGui::DragFloat3(metadata.name, metadata.obj->Raw(), 0.1f);
        }
    }
    else if constexpr (Meta::IsSame<MemberT, Vector4>)
    {
        ImGui::DragFloat4(metadata.name, metadata.obj->Raw(), 0.1f);
    }
    else if constexpr (Meta::IsSame<MemberT, Quaternion>)
    {
        // Convert to euler angles first
        Vector3 euler = Utils::GetQuaternionEulerAngles(*metadata.obj);
        ImGui::SliderAngle3(metadata.name, euler.Raw());

        // Then convert back to quaternion
        *metadata.obj = Quaternion::FromEuler(euler);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayColorType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Imgui only supports floating point color as input, so we need to convert to colorf then back to the correct type after
    if constexpr (Meta::IsSame<MemberT, ColorRgb>)
    {
        Colorf tmp = static_cast<Colorf>(*metadata.obj);
        ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *metadata.obj = static_cast<ColorRgb>(tmp);
    }
    else if constexpr (Meta::IsSame<MemberT, ColorHsv>)
    {
        Colorf tmp = static_cast<Colorf>(*metadata.obj);
        ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *metadata.obj = static_cast<ColorHsv>(tmp);
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
    // Get type of the pointer
    using PtrT = typename MemberT::Type;

    ImGui::PushID(metadata.obj);
    
    ImGui::Text("%s", metadata.name);

    // Special case for resources
    if constexpr (Meta::IsBaseOf<Resource, PtrT>)
    {
        ImGui::SameLine();

        // Display resource name
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

        // Pop up context to clear the resource
        if (ImGui::BeginPopupContextItem("RemoveResPopUp"))
        {
            if (ImGui::Selectable("Remove"))
                *metadata.obj = nullptr;

            ImGui::EndPopup();
        }

        // Mark as a drag drop target to receive a payload from the content browser
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("ContentBrowserFile");
                
            if (payload)
            {
                // Get the received resource
                Pointer<Resource> dragged = *static_cast<Pointer<Resource>*>(payload->Data);

                // Get the raw pointer for type checks
                const Resource* raw = static_cast<Resource*>(dragged);

                // Check that the dropped resource is a of the correct type
                if (Utils::GetTypeHash<Resource>(raw) == Utils::GetTypeHash<PtrT>())
                {
                    // Set resource
                    *reinterpret_cast<decltype(dragged)*>(metadata.obj) = dragged;
                }
            }
                
            ImGui::EndDragDropTarget();
        }
        
        ImGui::SameLine();

        if (ImGui::Button("+"))
        {
            // Set current object as the filter target
            m_ResourceFilterTarget = static_cast<void*>(metadata.obj);
            m_TextFilter.Clear();
        }

        // Check if the filter should be displayed
        if (m_ResourceFilterTarget == static_cast<void*>(metadata.obj))
        {
            Pointer<PtrT> res = FilterResources<PtrT>(m_TextFilter);
            if (res)
            {
                // Set value
                *metadata.obj = res;
                // Remove target
                m_ResourceFilterTarget = nullptr;
            }
        }
    }

    ImGui::PopID();
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayRawPointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Get type of the pointer
    using TypeT = Meta::RemovePointerSpecifier<MemberT>;

    if constexpr (Meta::IsSame<TypeT, Entity>)
    {
        // Handle an entity pointer

        ImGui::Text("%s", metadata.name);
        ImGui::SameLine();

        // Display entity name
        if (*metadata.obj != nullptr)
            ImGui::Text("%s", (*metadata.obj)->name.c_str());
        else
            ImGui::Text("No entity");

        // Pop up context to clear the entity
        if (ImGui::BeginPopupContextItem("RemoveEntityPopUp"))
        {
            if (ImGui::Selectable("Remove"))
                *metadata.obj = nullptr;

            ImGui::EndPopup();
        }

        // Mark as a drag drop target to receive a payload from the hierarchy window
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("HierarchyEntity");
        
            if (payload)
            {
                // Get the received entity
                Entity* const dragged = *static_cast<Entity**>(payload->Data);

                // Set value
                *metadata.obj = dragged;
            }
            
            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();

        if (ImGui::Button("+"))
        {
            // Set current object as the filter target
            m_EntityFilterTarget = static_cast<void*>(metadata.obj);
            m_TextFilter.Clear();
        }

        // Check if the filter should be displayed
        if (m_EntityFilterTarget == static_cast<void*>(metadata.obj))
        {
            Entity* e = FilterEntity(m_TextFilter);
            if (e)
            {
                // Set value
                *metadata.obj = e;
                // Remove target
                m_EntityFilterTarget = nullptr;
            }
        }
    }
    else if constexpr (Meta::IsSame<TypeT, Component>)
    {
        // TODO make generic for abstract pointers
        // Handle a component pointer
    
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
    // Get an array of the enum names
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    // Get the type, we can't know before compile time the type because the size of the array is determined when the function is processed
    using NamesArrayT = decltype(enumNames);

    // Getter for imgui
    constexpr auto getter = [](void* const userData, const int32_t idx) -> const char_t*
    {
        const auto ptr = static_cast<NamesArrayT*>(userData);
        return ptr->at(idx).data();
    };

    void* const names = reinterpret_cast<void*>(const_cast<Meta::RemoveConstSpecifier<NamesArrayT>*>(&enumNames));
    ImGui::Combo(metadata.name, reinterpret_cast<int32_t*>(metadata.obj), getter, names, static_cast<int32_t>(enumNames.size()));
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayEnumFlag(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Get an array of the enum names
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    constexpr size_t size = enumNames.size();

    // Compute preview value, it should display as all the selected values, comma separated
    // e.g. : Value1, Value2, Value8
    std::string previewValue;
    for (size_t i = 0; i < size; i++)
    {
        const size_t value = static_cast<size_t>(*metadata.obj);
        // Get current value
        const MemberT enumValue = magic_enum::enum_value<MemberT>(i);

        // Check value is set
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

    // Need to do a custom combo implementation because we can select multiple values
    if (ImGui::BeginCombo(metadata.name, previewValue.c_str()))
    {
        for (size_t i = 0; i < size; i++)
        {
            // Get enum value
            const size_t enumValue = static_cast<size_t>(magic_enum::enum_value<MemberT>(i));
            // Get member value
            size_t value = static_cast<size_t>(*metadata.obj);
            // Check if the enum value is set
            const bool_t isSelected = (value & enumValue) != 0;
            // Make a copy to pass the value to imgui, because we need to original value unchanged later
            bool_t tmpSelected = isSelected;

            if (ImGui::MenuItem(enumNames.at(i).data(), nullptr, &tmpSelected))
            {
                // Special case
                if (enumValue == 0)
                {
                    // Full clear value
                    value = 0;
                }
                else
                {
                    if (isSelected)
                    {
                        // Value was previously set, remove it
                        value &= ~enumValue;
                    }
                    else
                    {
                        // Value was previously not set, add it
                        value |= enumValue;
                    }
                }

                // Update value
                *metadata.obj = static_cast<MemberT>(value);
            }
        }
        
        ImGui::EndCombo();
    }
}

template <typename ReflectT>
void TypeRenderer::DisplayObject(ReflectT* const obj)
{
    // Get reflected data
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    // Humanized type name
    const std::string typeName = Utils::RemoveNamespaces(desc.name.c_str());

    // Display the type name centered
    const float_t textSize = ImGui::CalcTextSize(typeName.c_str()).x;
    Utils::AlignImGuiCursor(textSize);
    ImGui::Text("%s", typeName.c_str());

    // Loop over each reflected member
    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        // Get member type
        using MemberT = typename T::value_type;

        // Shorthand for the notify change attribute
        using NotifyChangeT = Reflection::NotifyChange<ReflectT>;
    
        constexpr bool_t hidden = Reflection::HasAttribute<Reflection::HideInInspector>(member);
        constexpr bool_t notifyChange = Reflection::HasAttribute<NotifyChangeT>(member);

        if constexpr (!hidden)
        {
            if constexpr (notifyChange)
            {
                // Need to notify if a change happened, so keep the old value
                const MemberT oldValue = member.get(obj);

                // Display object
                DisplayObjectInternal<ReflectT, MemberT, T>(obj, member);

                // Get the new value
                const MemberT newValue = member.get(obj);

                if (newValue != oldValue)
                {
                    // Value was changed, set the pointer to true
                    const auto notify = Reflection::GetAttribute<NotifyChangeT>(member);
                    obj->*notify.pointer = true;
                }
            }
            else
            {
                // Don't need to notify, simply display the object
                DisplayObjectInternal<ReflectT, MemberT, T>(obj, member);
            }
        }
    });
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayObjectInternal(ReflectT* obj, DescriptorT member)
{
    // Get humanized variable name
    const std::string n = Utils::HumanizeVariableName(member.name.c_str());
    const char_t* const name = n.c_str();

    // Construct metadata
    const Metadata<ReflectT, MemberT, DescriptorT> metadata = {
        .topLevelObj = obj,
        .name = name,
        .obj = &member.get(obj),
        .descriptor = member,
        .range = Reflection::TryGetAttribute<Reflection::Range<MemberT>, DescriptorT>(member)
    };

    if constexpr (Meta::IsArray<MemberT>)
    {
        // Native array
        DisplayArray<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsXnorList<MemberT>)
    {
        // Custom list type
        DisplayList<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else
    {
        // Simple type
        DisplaySimpleType<ReflectT, MemberT, DescriptorT>(metadata);
    }

    // Check for a tooltip
    CheckDisplayTooltip<ReflectT, MemberT, DescriptorT>(metadata);
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplaySimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Loop over the possible simple types
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
        // It wasn't in fact a simple type, which means it's a nested type, aka a struct or a class, so me display recursively
        if (ImGui::CollapsingHeader(metadata.name))
            DisplayObject<MemberT>(metadata.obj);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayArray(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Get array type
    using ArrayT = Meta::RemoveArraySpecifier<MemberT>;

    // Compute static array size
    constexpr size_t arraySize = sizeof(MemberT) / sizeof(ArrayT);

    if (ImGui::CollapsingHeader(metadata.name))
    {
        // Construct common metadata for the array type, this allows a range attribute to be used
        Metadata<ReflectT, ArrayT, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = "",
            .descriptor = metadata.descriptor,
            .range = Reflection::TryGetAttribute<Reflection::Range<ArrayT>>(metadata.descriptor)
        };
        
        for (size_t i = 0; i < arraySize; i++)
        {
            ImGui::PushID(&i + i);
            
            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            DisplaySimpleType<ReflectT, ArrayT, DescriptorT>(metadataArray);

            ImGui::PopID();
        }
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayList(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Get list type
    using ArrayT = typename MemberT::Type;
    // A list of component is a special case
    constexpr bool_t isComponentList = Meta::IsSame<ArrayT, Component*>;

    if (ImGui::CollapsingHeader(metadata.name))
    {
        if (ImGui::Selectable("Add"))
        {
            if constexpr (isComponentList)
            {
                // Set the target for the component filter
                m_ComponentFilterTarget = static_cast<void*>(metadata.obj);
                m_TextFilter.Clear();         
            }
            else
            {
                // "Normal" type, simply add an element to the list
                metadata.obj->Add();
            }
        }

        // Construct common metadata for the array type, this allows a range attribute to be used
        Metadata<ReflectT, ArrayT, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = "",
            .descriptor = metadata.descriptor,
            .range = Reflection::TryGetAttribute<Reflection::Range<ArrayT>>(metadata.descriptor)
        };

        size_t listSize = metadata.obj->GetSize();

        for (size_t i = 0; i < listSize; i++)
        {
            ImGui::PushID(&i + i);

            // Button to delete the current element
            if (ImGui::Button("-"))
            {
                metadata.obj->RemoveAt(i);
                --listSize;

                // Check for special cases :
                // - The element we deleted was the only one in the list, so the list is now empty
                // - The element we deleted was the last one in the list, so we don't have anything else to process
                if (listSize == 0 || i == listSize)
                {
                    ImGui::PopID();
                    break;
                }
            }
                
            ImGui::SameLine();

            // We can't insert an element in a component list
            if constexpr (!isComponentList)
            {
                if (ImGui::Button("+"))
                {
                    // Insert a new element at the given position
                    metadata.obj->Insert(i);
                    ++listSize;
                }
            }
                
            ImGui::SameLine();

            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            DisplaySimpleType<ReflectT, ArrayT, DescriptorT>(metadataArray);

            ImGui::PopID();
        }

        // Only a component list needs a filter
        if constexpr (isComponentList)
        {
            // Check if the filter should be displayed
            if (m_ComponentFilterTarget == static_cast<void*>(metadata.obj))
            {
                Component* const e = FilterComponent(m_TextFilter);
                if (e)
                {
                    metadata.obj->Add(e);
                    // Set entity pointer
                    e->entity = metadata.topLevelObj;
                    m_ComponentFilterTarget = nullptr;
                }
            }
        }
    }
}


template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::CheckDisplayTooltip(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    // Check if it has the attribute
    const Reflection::Tooltip* const tooltip = Reflection::TryGetAttribute<Reflection::Tooltip, DescriptorT>(metadata.descriptor);
    if (tooltip)
    {
        // Set tooltip        
        ImGui::SetItemTooltip("%s", tooltip->text);
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
        [&](const Pointer<T> r) -> bool_t
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
