#pragma once

#include "core.hpp"

#include <functional>

BEGIN_XNOR_CORE

class Factory
{
    STATIC_CLASS(Factory)

    struct FactoryTypeInfo
    {
        std::function<void*()> createFunc;
        std::function<void(void*)> displayFunc;
        std::function<void(void*)> serializeFunc;
        std::string name;
        std::vector<size_t> parentClasses;
    };
    
public:
    template <typename T>
    static constexpr void RegisterFactoryType();

    static void* CreateObject(size_t hash);
    static void* CreateObject(std::string name);

    static void DisplayObject(void* obj, size_t hash);

    template <typename T>
    static void FindAllChildClasses(std::vector<std::string>* names);

private:
    static bool_t IsChildOf(size_t typeHash, size_t parentHash);
    
    static inline std::unordered_map<size_t, FactoryTypeInfo> m_FactoryMapHash;
    static inline std::unordered_map<std::string, FactoryTypeInfo> m_FactoryMapName;
};

END_XNOR_CORE

#include "utils/factory.inl"
