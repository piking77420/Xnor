#pragma once

#include <functional>
#include <unordered_map>

#include "core.hpp"
#include "Coral/ManagedObject.hpp"
#include "scene/component/script_component.hpp"

/// @file dotnet_reflection.hpp
/// @brief Defines the XnorCore::DotnetReflection class.

BEGIN_XNOR_CORE

/// @brief Provides utility functions for dotnet types reflection
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
    /// @brief Prints the contents of the dotnet factory
    XNOR_ENGINE static void PrintTypes();

    /// @brief Registers a base type
    /// @tparam T Type
    /// @param typeName C# type name
    template <typename T>
    static void RegisterBaseType(const std::string& typeName);

    /// @brief Registers a core type
    /// @tparam T Type
    /// @param typeName C# type name
    template <typename T>
    static void RegisterCoreType(const std::string& typeName);

    /// @brief Registers a script type type
    /// @param typeName C# type name
    XNOR_ENGINE static void RegisterScriptType(const std::string& typeName);

    /// @brief Displays a simple type
    /// @tparam T Type
    /// @param obj Object pointer
    /// @param name Field name
    template <typename T>
    static void DisplaySimpleType(T* obj, const char_t* name);
    
    /// @brief Helper function to register all the base types
    XNOR_ENGINE static void RegisterAllTypes();

    /// @brief Displays a type using the internal dotnet factory
    /// @param obj Object pointer
    /// @param name Field name
    /// @param typeName C# type name
    XNOR_ENGINE static void DisplayType(void* obj, const char_t* name, const std::string& typeName);

    /// @brief Displays a script component type
    /// @param script Script component
    XNOR_ENGINE static void DisplayType(ScriptComponent* script);

private:
    static inline std::unordered_map<std::string, DotnetTypeInfo> m_DotnetMap; 
};

END_XNOR_CORE

#include "reflection/dotnet_reflection.inl"
