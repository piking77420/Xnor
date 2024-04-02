#include "reflection/dotnet_reflection.hpp"

#include "csharp/dotnet_assembly.hpp"
#include "csharp/dotnet_runtime.hpp"
#include "rendering/light/light.hpp"
#include "utils/logger.hpp"
#include "Maths/vector2i.hpp"

using namespace XnorCore;

void DotnetReflection::PrintTypes()
{
    const Coral::ManagedAssembly* const coreAssembly = DotnetAssembly::xnorCoreAssembly->GetCoralAssembly();
    const Coral::ManagedAssembly* const gameAssembly = DotnetRuntime::GetAssembly("Game")->GetCoralAssembly();

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
    const Coral::ManagedAssembly* const gameAssembly = DotnetRuntime::GetAssembly("Game")->GetCoralAssembly();

    Coral::Type& type = gameAssembly->GetType(typeName);
    DotnetTypeInfo info = {
        .createFunc = [&]() -> Coral::ManagedObject { return type.CreateInstance(); },
        .displayFunc = [](void* const, const char_t* const) -> void {},
        .serializeFunc = [](const void* const) -> void {},
        .deserializeFunc = [](void* const) -> void {},
        .name = typeName,
        .isScriptType = true
    };

    m_DotnetMap.emplace(typeName, info);
}

void DotnetReflection::RegisterAllTypes()
{
    RegisterBaseType<bool_t>("System.Boolean");
    RegisterBaseType<uint8_t>("System.Byte");
    RegisterBaseType<int8_t>("System.SByte");
    RegisterBaseType<char_t>("System.Char");
    RegisterBaseType<double_t>("System.Double");
    RegisterBaseType<float_t>("System.Single");
    RegisterBaseType<int32_t>("System.Int32");
    RegisterBaseType<uint32_t>("System.UInt32");
    RegisterBaseType<int64_t>("System.Int64");
    RegisterBaseType<uint64_t>("System.UInt64");
    RegisterBaseType<int16_t>("System.Int16");
    RegisterBaseType<uint16_t>("System.UInt16");

    RegisterBaseType<ColorHsva>(DotnetAssembly::XnorCoreNamespace + ".ColorHsva");
    RegisterBaseType<Colorf>(DotnetAssembly::XnorCoreNamespace + ".Colorf");
    RegisterBaseType<ColorRgb>(DotnetAssembly::XnorCoreNamespace + ".ColorRgb");
    RegisterBaseType<ColorRgba>(DotnetAssembly::XnorCoreNamespace + ".ColorRgba");

    RegisterBaseType<Vector2i>(DotnetAssembly::XnorCoreNamespace + ".Vector2i");
    RegisterBaseType<Vector2>(DotnetAssembly::XnorCoreNamespace + ".Vector2");
    RegisterBaseType<Vector3>(DotnetAssembly::XnorCoreNamespace + ".Vector3");
    RegisterBaseType<Vector4>(DotnetAssembly::XnorCoreNamespace + ".Vector4");

    RegisterCoreType<Light>(DotnetAssembly::XnorCoreNamespace + ".Light");
}

void DotnetReflection::DisplayType(void* obj, const char_t* const name, const std::string& typeName)
{
    auto&& it = m_DotnetMap.find(typeName);

    if (it == m_DotnetMap.end())
    {
        Logger::LogError("[C# Refl] : Couldn't find type named {}", typeName);
        return;
    }

    it->second.displayFunc(obj, name);
}

void DotnetReflection::DisplayType(ScriptComponent* const script)
{
    Coral::ManagedObject& obj = script->GetManagedObject();

    ImGui::PushID(script);
    for (Coral::FieldInfo& field : obj.GetType().GetFields())
    {
        const std::string fieldName = field.GetName();
        const std::string typeName = field.GetType().GetFullName();

        void* const ptr = obj.GetFieldPointer<void>(fieldName);

        ImGui::PushID(ptr);
        DisplayType(ptr, fieldName.c_str(), typeName);
        ImGui::PopID();
    }
    ImGui::PopID();
}
