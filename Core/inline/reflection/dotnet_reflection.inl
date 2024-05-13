// ReSharper disable CppTooWideScope
#pragma once

#include "csharp/dotnet_runtime.hpp"
#include "serialization/serializer.hpp"

BEGIN_XNOR_CORE

template <typename T>
void DotnetReflection::RegisterBaseType(const std::string& typeName)
{
    DotnetTypeInfo info = {
        .createFunc = []() -> Coral::ManagedObject { return {}; },
        .displayFunc = [](void* const obj, const char_t* const name) -> void { DisplaySimpleType<T>(static_cast<T*>(obj), name); },
        .serializeFunc = [](void* const value, const std::string& fieldName) -> void { SerializeSimpleType<T>(static_cast<T*>(value), fieldName); },
        .deserializeFunc = [](void* const value) -> void { DeserializeSimpleType(value); },
        .name = typeName,
        .isScriptType = false
    };

    m_DotnetMap.emplace(typeName, info);
}

template <typename T>
void DotnetReflection::RegisterCoreType(const std::string& typeName)
{
    DotnetTypeInfo info = {
        .createFunc = []() -> Coral::ManagedObject { return {}; },
        .displayFunc = [](void* const, const char_t* const name) -> void { ImGui::Text("%s", name); },
        .serializeFunc = [](void* const, const std::string&) -> void { },
        .deserializeFunc = [](void* const) -> void { },
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
    else if constexpr (Meta::IsAny<T, uint8_t, char_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_U8, obj);
    }
    else if constexpr (Meta::IsSame<T, int8_t>)
    {
        ImGui::DragScalar(name, ImGuiDataType_S8, obj);
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
        ImGui::DragScalar(name, ImGuiDataType_U32, obj);
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
        Color tmp = static_cast<Color>(*obj);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *obj = static_cast<ColorHsva>(tmp);
    }
    else if constexpr (Meta::IsSame<T, Color>)
    {
        ImGui::ColorPicker4(name, reinterpret_cast<float_t*>(obj));
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

template <typename T>
void DotnetReflection::SerializeSimpleType(T* const obj, const std::string& name)
{
    if constexpr (Meta::IsNativeType<T> || Meta::IsMathType<T> || Meta::IsColorType<T> || Meta::IsSame<T, std::string>)
    {
        Serializer::AddSimpleValue(name, *obj);
    }
    else if constexpr (Meta::IsEnum<T>)
    {
        // TODO constexpr bool_t isEnumFlag = Reflection::HasAttribute<Reflection::EnumFlags, DescriptorT>();
        constexpr auto enumNames = magic_enum::enum_names<T>();
    
        if constexpr (false)
        {
            Serializer::AddSimpleValue<std::string>(name, magic_enum::enum_flags_name<T>(*obj, '|').data());
        }
        else
        {
            Serializer::AddSimpleValue<std::string>(name, magic_enum::enum_name<T>(*obj).data());
        }
    }
}

template <typename T>
void DotnetReflection::DeserializeSimpleType(T* const obj)
{
}

END_XNOR_CORE
