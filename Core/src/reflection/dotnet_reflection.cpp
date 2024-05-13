#include "reflection/dotnet_reflection.hpp"

#include <Maths/vector2i.hpp>

#include "csharp/dotnet_assembly.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "rendering/light/light.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void DotnetReflection::PrintTypes()
{
    const Coral::ManagedAssembly* const coreAssembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();
    const Coral::ManagedAssembly* const gameAssembly = DotnetRuntime::GetGameAssembly()->GetCoralAssembly();

    RegisterAllTypes();

    const Coral::Type& scriptComponentType = coreAssembly->GetType(DotnetAssembly::XnorCoreNamespace + ".ScriptComponent");

    for (const Coral::Type* type : gameAssembly->GetTypes())
    {
        if (type->IsSubclassOf(scriptComponentType))
        {
            RegisterScriptType(type->GetFullName());
        }
    }

    for (auto&& it : m_DotnetMap)
    {
        Logger::LogInfo("{} ; {}", it.first, it.second.name);
    }
}

void DotnetReflection::RegisterScriptType(const std::string& typeName)
{
    const Coral::ManagedAssembly* const gameAssembly = DotnetRuntime::GetGameAssembly()->GetCoralAssembly();

    Coral::Type& type = gameAssembly->GetType(typeName);
    DotnetTypeInfo info = {
        .createFunc = [&]() -> Coral::ManagedObject { return type.CreateInstance(); },
        .displayFunc = [&, tName = typeName](void* const value, const char_t* fieldName) -> void { DisplayExternalType(value, fieldName, tName); },
        .serializeFunc = [&, tName = typeName](void* const value, const std::string& fieldName) -> void { SerializeExternalType(value, fieldName, tName); },
        .deserializeFunc = [](void* const) -> void {},
        .name = typeName,
        .isScriptType = true
    };

    m_DotnetMap.emplace(typeName, info);
}

void DotnetReflection::RegisterEnumType(const std::string& typeName, const std::string& assemblyName)
{
    const Coral::ManagedAssembly* const gameAssembly = DotnetRuntime::GetAssembly(assemblyName)->GetCoralAssembly();

    Coral::Type& type = gameAssembly->GetType(typeName);
    DotnetTypeInfo info = {
        .createFunc = [&]() -> Coral::ManagedObject { return type.CreateInstance(); },
        .displayFunc = [&, tName = typeName](void* const value, const char_t* fieldName) -> void { DisplayExternalType(value, fieldName, tName); },
        .serializeFunc = [&, tName = typeName](void* const value, const std::string& fieldName) -> void { SerializeExternalType(value, fieldName, tName); },
        .deserializeFunc = [](void* const) -> void {},
        .name = typeName,
        .isScriptType = false
    };

    m_DotnetMap.emplace(typeName, info);
}

void DotnetReflection::UnregisterScriptType(const std::string& typeName)
{
    m_DotnetMap.erase(m_DotnetMap.find(typeName));
}

void DotnetReflection::RegisterAllTypes()
{
    RegisterBaseType<bool_t>("System.Boolean");
    RegisterBaseType<uint8_t>("System.Byte");
    RegisterBaseType<int8_t>("System.SByte");
    RegisterBaseType<int16_t>("System.Int16");
    RegisterBaseType<uint16_t>("System.UInt16");
    RegisterBaseType<int32_t>("System.Int32");
    RegisterBaseType<uint32_t>("System.UInt32");
    RegisterBaseType<int64_t>("System.Int64");
    RegisterBaseType<uint64_t>("System.UInt64");
    RegisterBaseType<char_t>("System.Char");
    RegisterBaseType<double_t>("System.Double");
    RegisterBaseType<float_t>("System.Single");

    static const std::string XnorCoreNamespace = DotnetAssembly::XnorCoreNamespace + '.';

    RegisterBaseType<ColorHsva>(XnorCoreNamespace + "ColorHsva");
    RegisterBaseType<Color>(XnorCoreNamespace + "Colorf");

    RegisterBaseType<Vector2i>(XnorCoreNamespace + "Vector2i");
    RegisterBaseType<Vector2>(XnorCoreNamespace + "Vector2");
    RegisterBaseType<Vector3>(XnorCoreNamespace + "Vector3");
    RegisterBaseType<Vector4>(XnorCoreNamespace + "Vector4");

    RegisterCoreType<Light>(XnorCoreNamespace + "Light");
}

void DotnetReflection::DisplayType(void* obj, const char_t* const name, const std::string& typeName)
{
    if (DotnetRuntime::IsReloadingProject())
    {
        ImGui::TextColored(static_cast<Vector4>(Color::Gray()), ".NET scripts are reloading...");
        return;
    }

    auto&& it = m_DotnetMap.find(typeName);

    if (it == m_DotnetMap.end())
    {
        Logger::LogError("[C# Refl] Couldn't find type named {}", typeName);
        return;
    }

    it->second.displayFunc(obj, name);
}

void DotnetReflection::DisplayType(ScriptComponent* const script)
{
    if (DotnetRuntime::IsReloadingProject())
    {
        ImGui::TextColored(static_cast<Vector4>(Color::Gray()), ".NET scripts are reloading...");
        return;
    }
    
    Coral::ManagedObject& obj = script->GetManagedObject();

    ImGui::PushID(script);
    for (Coral::FieldInfo& field : obj.GetType().GetFields())
    {
        const std::string fieldName = field.GetName();

        if (Utils::ArrayContains(IgnoredFieldNames, fieldName))
            continue;
        
        const std::string typeName = field.GetType().GetFullName();

        void* const ptr = obj.GetFieldPointer<void>(fieldName);

        ImGui::PushID(ptr);
        DisplayType(ptr, fieldName.c_str(), typeName);
        ImGui::PopID();
    }
    ImGui::PopID();
}

void DotnetReflection::GetScriptTypes(List<std::string>* const list)
{
    for (DotnetTypeInfo& typeInfo : m_DotnetMap | std::views::values)
    {
        if (typeInfo.isScriptType)
            list->Add(typeInfo.name);
    }
}

ScriptComponent* DotnetReflection::CreateInstance(const std::string& typeName)
{
    auto&& obj = m_DotnetMap[typeName].createFunc();
    ScriptComponent* component = obj.GetFieldValue<ScriptComponent*>("swigCPtr");
    component->Initialize(obj);
    return component;
}

void DotnetReflection::SerializeType(void* const value, const std::string& fieldName, const std::string& typeName)
{
    auto&& it = m_DotnetMap.find(typeName);

    if (it == m_DotnetMap.end())
    {
        Logger::LogError("[C# Refl] Couldn't find type named {}", typeName);
        return;
    }

    it->second.serializeFunc(value, fieldName);
}

void DotnetReflection::SerializeExternalType(void* value, const std::string& fieldName, const std::string& typeName)
{
    const Coral::Type& type = DotnetRuntime::GetGameAssembly()->GetCoralAssembly()->GetType(typeName);
    
    if (type.IsEnum())
    {
        Serializer::AddSimpleValue(fieldName, *static_cast<int32_t* const>(value));
    }
}

void DotnetReflection::DisplayExternalType(void* const value, const char_t* const fieldName, const std::string& typeName)
{
    const Coral::Type& type = DotnetRuntime::GetGameAssembly()->GetCoralAssembly()->GetType(typeName);
    
    if (type.IsEnum())
    {
        int32_t* const val = static_cast<int32_t* const>(value);
        
        std::vector<Coral::String> enumNames;
        type.GetEnumNames(enumNames);
        
        std::vector<int32_t> enumValues;
        type.GetEnumValues(enumValues);

        if (ImGui::BeginCombo(fieldName, enumNames[std::distance(enumValues.begin(), std::ranges::find(enumValues, *val))].Data()))
        {
            for (size_t i = 0; i < enumNames.size(); i++)
            {
                if (ImGui::Selectable(enumNames[i].Data()))
                    *val = enumValues[i];
            }
            ImGui::EndCombo();
        }
    }
}

void DotnetReflection::SerializeScript(const ScriptComponent* const script)
{
    const Coral::ManagedObject& obj = script->GetManagedObject();
    const Coral::Type& type = obj.GetType();

    Serializer::AddAttribute("managedType", type.GetFullName());
    for (Coral::FieldInfo& field : type.GetFields())
    {
        const std::string fieldName = field.GetName();

        if (Utils::ArrayContains(IgnoredFieldNames, fieldName))
            continue;
        
        const std::string typeName = field.GetType().GetFullName();

        void* const ptr = const_cast<Coral::ManagedObject&>(obj).GetFieldPointer<void>(fieldName);

        SerializeType(ptr, fieldName, typeName);
    }
}

void DotnetReflection::DeserializeScript(ScriptComponent* const script)
{
}
