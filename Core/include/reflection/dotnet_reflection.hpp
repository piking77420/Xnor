#pragma once

#include <functional>
#include <unordered_map>

#include "core.hpp"
#include "Coral/ManagedObject.hpp"
#include "scene/component/script_component.hpp"

BEGIN_XNOR_CORE
    class DotnetReflection
{
    STATIC_CLASS(DotnetReflection)

    struct DotnetTypeInfo
    {
        std::function<Coral::ManagedObject()> createFunc;
        std::function<void(void*, const char_t*)> displayFunc;
        std::function<void(void*)> serializeFunc;
        std::function<void(void*)> deserializeFunc;

        std::string name;
        bool_t isScriptType;
    };

public:
    static void PrintTypes();

    template <typename T>
    static void RegisterBaseType(const std::string& typeName);

    template <typename T>
    static void DisplaySimpleType(T* obj, const char_t* name);

    static void RegisterScriptType(const std::string& typeName);
    
    static void RegisterNativeTypes();

    static void DisplayType(void* obj, const char_t* name, const std::string& typeName);
    XNOR_ENGINE static void DisplayType(ScriptComponent* script);

private:
    static inline std::unordered_map<std::string, DotnetTypeInfo> m_DotnetMap; 
};

END_XNOR_CORE

#include "reflection/dotnet_reflection.inl"
