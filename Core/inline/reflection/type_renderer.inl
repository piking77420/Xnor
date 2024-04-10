// ReSharper disable CppTooWideScope
#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "input/keyboard_input.hpp"
#include "magic_enum/magic_enum_all.hpp"
#include "reflection/filters.hpp"
#include "utils/utils.hpp"
#include "world/world.hpp"

BEGIN_XNOR_CORE

template <typename MemberT, typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<MemberT, T>::Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Reflection::IsReflected<MemberT>)
        TypeRenderer::DisplayObject<MemberT>(metadata.obj);
    else
        Logger::LogError("Type doesn't have a TypeRenderImpl : {} of type {}", metadata.name, typeid(MemberT).name());
}

template <typename MemberT>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<MemberT, Meta::EnableIf<Meta::IsEnum<MemberT>>>::Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Reflection::HasAttribute<Reflection::EnumFlags, DescriptorT>())
        TypeRenderer::DisplayEnumFlag<ReflectT, MemberT, DescriptorT>(metadata);
    else
        TypeRenderer::DisplayEnum<ReflectT, MemberT, DescriptorT>(metadata);
}

template <typename MemberT>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<MemberT, Meta::EnableIf<Meta::IsIntegralOrFloating<MemberT>>>::Render(const TypeRenderer::Metadata<ReflectT, MemberT, DescriptorT>& metadata)
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

    if constexpr (Reflection::HasAttribute<Reflection::Range<MemberT>, DescriptorT>())
    {
        const Reflection::Range<MemberT>& range = Reflection::GetAttribute<Reflection::Range<MemberT>, DescriptorT>();
        // Has a range attribute, display as a slider
        ImGui::SliderScalar(metadata.name, type, metadata.obj, &range.minimum, &range.maximum);        
    }
    else
    {
        // No range attribute, display as a scalar
        ImGui::DragScalar(metadata.name, type, metadata.obj, .1f);
    }
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<bool_t>::Render(const TypeRenderer::Metadata<ReflectT, bool_t, DescriptorT>& metadata)
{
    ImGui::Checkbox(metadata.name, metadata.obj);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector2>::Render(const TypeRenderer::Metadata<ReflectT, Vector2, DescriptorT>& metadata)
{
    if constexpr (Reflection::HasAttribute<Reflection::GridPlotting, DescriptorT>())
    {
        TypeRenderer::DisplayGridPlotting<ReflectT, Vector2, DescriptorT>(metadata);
    }
    else
    {
        ImGui::DragFloat2(metadata.name, metadata.obj->Raw(), 0.1f);
    }
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector2i>::Render(const TypeRenderer::Metadata<ReflectT, Vector2i, DescriptorT>& metadata)
{
    ImGui::DragInt2(metadata.name, metadata.obj->Raw(), 0.1f);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector3>::Render(const TypeRenderer::Metadata<ReflectT, Vector3, DescriptorT>& metadata)
{
    if constexpr (Reflection::HasAttribute<Reflection::AsEulerAngles, DescriptorT>())
    {
        // Supposed to be euler angles, so draw as a slider angle
        ImGui::SliderAngle3(metadata.name, metadata.obj->Raw(), -360, 360, "%.0f deg", ImGuiSliderFlags_AlwaysClamp);
    }
    else
    {
        ImGui::DragFloat3(metadata.name, metadata.obj->Raw(), 0.1f);
    }
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Vector4>::Render(const TypeRenderer::Metadata<ReflectT, Vector4, DescriptorT>& metadata)
{
    ImGui::DragFloat4(metadata.name, metadata.obj->Raw(), 0.1f);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Quaternion>::Render(const TypeRenderer::Metadata<ReflectT, Quaternion, DescriptorT>& metadata)
{
    // Convert to euler angles first
    Vector3 euler = Utils::GetQuaternionEulerAngles(*metadata.obj);
    ImGui::SliderAngle3(metadata.name, euler.Raw());

    // Then convert back to quaternion
    *metadata.obj = Quaternion::FromEuler(euler);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Colorf>::Render(const TypeRenderer::Metadata<ReflectT, Colorf, DescriptorT>& metadata)
{
    ImGui::ColorPicker4(metadata.name, reinterpret_cast<float_t*>(metadata.obj));
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<ColorRgb>::Render(const TypeRenderer::Metadata<ReflectT, ColorRgb, DescriptorT>& metadata)
{
    Colorf tmp = static_cast<Colorf>(*metadata.obj);
    ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
    *metadata.obj = static_cast<ColorRgb>(tmp);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<ColorHsva>::Render(const TypeRenderer::Metadata<ReflectT, ColorHsva, DescriptorT>& metadata)
{
    Colorf tmp = static_cast<Colorf>(*metadata.obj);
    ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
    *metadata.obj = static_cast<ColorHsva>(tmp);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<ColorRgba>::Render(const TypeRenderer::Metadata<ReflectT, ColorRgba, DescriptorT>& metadata)
{
    Colorf tmp = static_cast<Colorf>(*metadata.obj);
    ImGui::ColorPicker4(metadata.name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
    *metadata.obj = static_cast<ColorRgba>(tmp);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<std::string>::Render(const TypeRenderer::Metadata<ReflectT, std::string, DescriptorT>& metadata)
{
    ImGui::InputText(metadata.name, metadata.obj);
}

template <typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Pointer<T>, Meta::EnableIf<Meta::IsBaseOf<Resource, T>>>::Render(const TypeRenderer::Metadata<ReflectT, Pointer<T>, DescriptorT>& metadata)
{
    ImGui::PushID(metadata.obj);
    
    ImGui::Text("%s", metadata.name);

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

            // Check that the dropped resource is of the correct type
            if (Utils::GetTypeHash<Resource>(raw) == Utils::GetTypeHash<T>())
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
        Filters::BeginResourceFilter();
    }

    // Check if the filter should be displayed
    Filters::FilterResources<T>(metadata.obj);

    ImGui::PopID();
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Entity*>::Render(const TypeRenderer::Metadata<ReflectT, Entity*, DescriptorT>& metadata)
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
        Filters::BeginEntityFilter();
    }

    // Check if the filter should be displayed
    Filters::FilterEntity(metadata.obj);
}

template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<Component*>::Render(const TypeRenderer::Metadata<ReflectT, Component*, DescriptorT>& metadata)
{
    // TODO make generic for abstract pointers
    // Handle a component pointer
    
    const size_t hash = Utils::GetTypeHash<Component>(*metadata.obj);

    if (ImGui::CollapsingHeader(metadata.name))
    {
        TypeRenderer::DisplayObjectUsingFactory(*metadata.obj, hash);
    }
}

template <typename T, size_t N>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<T[N]>::Render(const TypeRenderer::Metadata<ReflectT, T[N], DescriptorT>& metadata)
{
    if (ImGui::CollapsingHeader(metadata.name))
    {
        // Construct common metadata for the array type, this allows a range attribute to be used
        TypeRenderer::Metadata<ReflectT, T, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = ""
        };
        
        for (size_t i = 0; i < N; i++)
        {
            ImGui::PushID(&i + i);
            
            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            TypeRenderer::DisplaySimpleType<ReflectT, T, DescriptorT>(metadataArray);

            ImGui::PopID();
        }
    }
}

template <typename T>
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<List<T>>::Render(const TypeRenderer::Metadata<ReflectT, List<T>, DescriptorT>& metadata)
{
    if (ImGui::CollapsingHeader(metadata.name))
    {
        if (ImGui::Selectable("Add"))
        {
            // Simply add an element to the list
            metadata.obj->Add();
        }

        // Construct common metadata for the array type, this allows a range attribute to be used
        TypeRenderer::Metadata<ReflectT, T, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = ""
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

            // Handle inserting elements
            if (ImGui::Button("+"))
            {
                // Insert a new element at the given position
                metadata.obj->Insert(i);
                ++listSize;
            }
                
            ImGui::SameLine();

            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            TypeRenderer::DisplaySimpleType<ReflectT, T, DescriptorT>(metadataArray);

            ImGui::PopID();
        }
    }
}
template <typename ReflectT, typename DescriptorT>
void TypeRendererImpl<List<Component*>>::Render(const TypeRenderer::Metadata<ReflectT, List<Component*>, DescriptorT>& metadata)
{
    if (ImGui::CollapsingHeader(metadata.name))
    {
        if (ImGui::Selectable("Add"))
        {
            // Start component filter
            Filters::BeginComponentFilter();
        }

        // Construct common metadata for the array type, this allows a range attribute to be used
        TypeRenderer::Metadata<ReflectT, Component*, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = ""
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

            ImGui::SameLine();

            const std::string index = std::to_string(i);

            // Feed the element specific data in the metadata
            metadataArray.name = index.c_str();
            metadataArray.obj = &(*metadata.obj)[i];

            // Display as a simple type
            TypeRenderer::DisplaySimpleType<ReflectT, Component*, DescriptorT>(metadataArray);

            ImGui::PopID();
        }

        // Process filter
        Component* const c = Filters::FilterComponent(metadata.obj);
        if (c != nullptr)
        {
            c->entity = metadata.topLevelObj;
        }
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayGridPlotting(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    const Reflection::GridPlotting& plotting = Reflection::GetAttribute<Reflection::GridPlotting, DescriptorT>();
    
    ImGui::Text("%s", metadata.name);
    ImDrawList* const drawList = ImGui::GetWindowDrawList();

    // TODO handle resize
    constexpr Vector2 size = Vector2(100, 100);
    ImGui::InvisibleButton("##canvas", size);

    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImVec2 p1 = ImGui::GetItemRectMax();

    const Vector2 plottingRange = Vector2(plotting.minimum, plotting.maximum);
    constexpr Vector2 uniformRange = Vector2(0.f, 1.f);

    // Handle clicking
    if (ImGui::IsItemActive() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        const ImGuiIO& io = ImGui::GetIO();

        // Compute new value, ranged between 0 and 1
        const Vector2 newValue = (io.MousePos - p0) / size;

        // Remap the value from [0; 1] to [min; max]
        metadata.obj->x = Utils::RemapValue(newValue.x, uniformRange, plottingRange);
        metadata.obj->y = Utils::RemapValue(newValue.y, uniformRange, plottingRange);

        // Clamp the value between min and max
        metadata.obj->x = std::clamp<float_t>(metadata.obj->x, plotting.minimum, plotting.maximum);
        metadata.obj->y = std::clamp<float_t>(metadata.obj->y, plotting.minimum, plotting.maximum);
    }

    // Create rectangle
    ImGui::PushClipRect(p0, p1, true);
    drawList->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));

    // Remap from [min; max] to [0, 1]
    const Vector2 clamped = Vector2(
        Utils::RemapValue(metadata.obj->x, plottingRange, uniformRange),
        Utils::RemapValue(metadata.obj->y, plottingRange, uniformRange)
    );

    // Compute cursor position
    const Vector2 position = p0 + clamped * size;

    drawList->AddCircle(position, 5, IM_COL32_WHITE);
    ImGui::PopClipRect();

    // Draw slider float version
    ImGui::SameLine();
    ImGui::VSliderFloat("##v2y", ImVec2(18, 100), &metadata.obj->y, plotting.minimum, plotting.maximum, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("##v2x", &metadata.obj->x, plotting.minimum, plotting.maximum, "%.3f", ImGuiSliderFlags_AlwaysClamp);
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
    // e.g. : Value1,Value2,Value8
    auto v = magic_enum::enum_flags_name<MemberT>(*metadata.obj, ',');
    const char_t* const previewValue = v.data();

    // Need to do a custom combo implementation because we can select multiple values
    if (ImGui::BeginCombo(metadata.name, previewValue))
    {
        for (size_t i = 0; i < size; i++)
        {
            // Get enum value
            const size_t enumValue = static_cast<size_t>(magic_enum::enum_value<MemberT>(i));
            // Get member value
            size_t value = static_cast<size_t>(*metadata.obj);
            // Check if the enum value is set
            const bool_t isSelected = (value & enumValue) != 0;

            if (ImGui::MenuItem(enumNames.at(i).data(), nullptr, isSelected))
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

    DisplayFields<ReflectT, true>(obj);
    DisplayFields<ReflectT, false>(obj);
}

template <typename ReflectT, bool_t IsStatic>
void TypeRenderer::DisplayFields(ReflectT* const obj)
{
    // Get reflected data
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    bool_t hasStatic = false;

    // Loop over each reflected member
    refl::util::for_each(desc.members, [&]<typename DescriptorT>(const DescriptorT)
    {
        // Get member type
        using MemberT = Meta::RemoveConstSpecifier<typename DescriptorT::value_type>;

        // Shorthand for the notify change attribute
        using NotifyChangeT = Reflection::NotifyChange<ReflectT>;

        constexpr bool_t isConst = !DescriptorT::is_writable;
        constexpr bool_t hidden = Reflection::HasAttribute<Reflection::HideInInspector, DescriptorT>();
        constexpr bool_t readOnly = Reflection::HasAttribute<Reflection::ReadOnly, DescriptorT>();

        // We want to display static fields when IsStatic is true, and member fields when IsStatic is false
        // Hence the binary operations
        // Truth table :
        // IsStatic | is_static | !is_static | Result
        // 0        | 0         | 1          | 1
        // 0        | 1         | 0          | 0
        // 1        | 0         | 1          | 1
        // 1        | 1         | 0          | 0
        constexpr bool_t display = IsStatic ^ !DescriptorT::is_static;

        if constexpr (IsStatic && DescriptorT::is_static)
        {
            hasStatic = true;
        }

        constexpr bool_t notifyChange = Reflection::HasAttribute<NotifyChangeT, DescriptorT>();

        if constexpr (!hidden && display)
        {
            ImGui::BeginDisabled(isConst || readOnly);
            
            if constexpr (notifyChange)
            {
                // Need to notify if a change happened, so keep the old value
                const MemberT oldValue = DescriptorT::get(obj);

                // Display object
                DisplayObjectInternal<ReflectT, MemberT, DescriptorT>(obj);

                // Get the new value
                const MemberT newValue = DescriptorT::get(obj);

                if (newValue != oldValue)
                {
                    // Value was changed, set the pointer to true
                    constexpr NotifyChangeT notify = Reflection::GetAttribute<NotifyChangeT, DescriptorT>();
                    obj->*notify.pointer = true;
                }
            }
            else
            {
                // Don't need to notify, simply display the object
                DisplayObjectInternal<ReflectT, MemberT, DescriptorT>(obj);
            }

            ImGui::EndDisabled();
        }
    });

    if constexpr (IsStatic)
    {
        if (hasStatic)
        {
            ImGui::Separator();
        }
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplayObjectInternal(ReflectT* obj)
{
    // Get humanized variable name
    const std::string n = Utils::HumanizeVariableName(DescriptorT::name.c_str());
    const char_t* const name = n.c_str();

    // Construct metadata
    const Metadata<ReflectT, MemberT, DescriptorT> metadata = {
        .topLevelObj = obj,
        .name = name,
        .obj = [&]() -> MemberT*
        {
            if constexpr (DescriptorT::is_static)
                return const_cast<MemberT*>(&DescriptorT::get());
            else
                return const_cast<MemberT*>(&DescriptorT::get(obj));
        }()
    };

    ImGui::PushID(metadata.obj);

    DisplaySimpleType<ReflectT, MemberT, DescriptorT>(metadata);

    // Check for a tooltip
    CheckDisplayTooltip<ReflectT, MemberT, DescriptorT>(metadata);

    ImGui::PopID();
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::DisplaySimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    TypeRendererImpl<MemberT>::template Render<ReflectT, DescriptorT>(metadata);
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void TypeRenderer::CheckDisplayTooltip(const Metadata<ReflectT, MemberT, DescriptorT>&)
{
    // Check if it has the attribute
    if constexpr (Reflection::HasAttribute<Reflection::Tooltip, DescriptorT>())
    {
        // Set tooltip        
        ImGui::SetItemTooltip("%s", Reflection::GetAttribute<Reflection::Tooltip, DescriptorT>().text);
    }
}

END_XNOR_CORE
