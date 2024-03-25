// ReSharper disable CppTooWideScope
#pragma once

#include "csharp/dotnet_runtime.hpp"
#include "scene/component/script_component.hpp"

BEGIN_XNOR_CORE

template <typename T>
void DotnetReflection::RegisterBaseType(const std::string& typeName)
{
    constexpr bool_t isScript = Meta::IsBaseOf<ScriptComponent, T>;
    
    DotnetTypeInfo info = {
        .createFunc = []() -> Coral::ManagedObject { return {}; },
        .displayFunc = [](void* const obj, const char_t* const name) -> void { DisplaySimpleType<T>(static_cast<T*>(obj), name); },
        .serializeFunc = [](const void* const) -> void {},
        .deserializeFunc = [](void* const) -> void {},
        .name = typeName,
        .isScriptType = false
    };

    m_DotnetMap.emplace(typeName, info);
}

template <typename T>
void DotnetReflection::DisplaySimpleType(T* const obj, const char_t* const name)
{
    if constexpr (Meta::IsSame<T, bool_t>)
    {
        ImGui::Checkbox(name, obj);
    }
    else if constexpr (Meta::IsSame<T, uint8_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U8, obj);
    }
    else if constexpr (Meta::IsSame<T, int8_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S8, obj);
    }
    else if constexpr (Meta::IsSame<T, char_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U8, obj);
    }
    else if constexpr (Meta::IsSame<T, double_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_Double, obj);
    }
    else if constexpr (Meta::IsSame<T, float_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_Float, obj);
    }
    else if constexpr (Meta::IsSame<T, int32_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S32, obj);
    }
    else if constexpr (Meta::IsSame<T, uint32_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S32, obj);
    }
    else if constexpr (Meta::IsSame<T, int64_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S64, obj);
    }
    else if constexpr (Meta::IsSame<T, uint64_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U64, obj);
    }
    else if constexpr (Meta::IsSame<T, int16_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S16, obj);
    }
    else if constexpr (Meta::IsSame<T, uint16_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U16, obj);
    }
    else if constexpr (Meta::IsSame<T, ColorHsva>)
    {
        Colorf tmp = static_cast<Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *obj = static_cast<ColorHsva>(tmp);
    }
    else if constexpr (Meta::IsSame<T, Colorf>)
    {
        ImGui::ColorPicker4(name, reinterpret_cast<float_t*>(obj));
    }
    else if constexpr (Meta::IsSame<T, ColorRgb>)
    {
        Colorf tmp = static_cast<Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *obj = static_cast<ColorRgb>(tmp);
    }
    else if constexpr (Meta::IsSame<T, ColorRgba>)
    {
        Colorf tmp = static_cast<Colorf>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *obj = static_cast<ColorRgba>(tmp);
    }
    else if constexpr (Meta::IsSame<T, Vector2i>)
    {
        ImGui::DragInt2(name, obj->Raw(), .1f);
    }
    else if constexpr (Meta::IsSame<T, Vector2>)
    {
        ImGui::DragFloat2(name, obj->Raw(), .1f);
    }
    else if constexpr (Meta::IsSame<T, Vector3>)
    {
        ImGui::DragFloat3(name, obj->Raw(), .1f);
    }
    else if constexpr (Meta::IsSame<T, Vector4>)
    {
        ImGui::DragFloat4(name, obj->Raw(), .1f);
    }
}

END_XNOR_CORE
