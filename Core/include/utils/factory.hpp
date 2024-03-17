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
        const char_t* name;
        std::vector<size_t> parentClasses;
    };
    
public:
    template <typename T>
    static constexpr void RegisterFactoryType();

    XNOR_ENGINE static void* CreateObject(size_t hash);
    XNOR_ENGINE static void* CreateObject(const char_t* name);

    XNOR_ENGINE static void DisplayObject(void* obj, size_t hash);

    template <typename T>
    static void FindAllChildClasses(std::vector<const char_t*>* names);

private:
    XNOR_ENGINE static bool_t IsChildOf(size_t typeHash, size_t parentHash);
    
    XNOR_ENGINE static inline std::unordered_map<size_t, FactoryTypeInfo> m_FactoryMapHash;
    XNOR_ENGINE static inline std::unordered_map<const char_t*, FactoryTypeInfo> m_FactoryMapName;
};

END_XNOR_CORE

#include "utils/factory.inl"
