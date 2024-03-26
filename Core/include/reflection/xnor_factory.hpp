#pragma once

#include "core.hpp"

#include <functional>

BEGIN_XNOR_CORE

class XnorFactory
{
    STATIC_CLASS(XnorFactory)

    struct FactoryTypeInfo
    {
        std::function<void*()> createFunc;
        std::function<void(void*)> displayFunc;
        std::function<void(void*)> serializeFunc;
        std::function<void(void*)> deserializeFunc;

        bool_t isConstructible;
        std::string name;
        std::vector<size_t> parentClasses;
    };
    
public:
    template <typename T>
    static constexpr void RegisterFactoryType();

    XNOR_ENGINE static void* CreateObject(size_t hash);
    XNOR_ENGINE static void* CreateObject(const std::string& name);
    XNOR_ENGINE static void DisplayObject(void* obj, size_t hash);

    XNOR_ENGINE static void SerializeObject(void* obj, size_t hash);
    XNOR_ENGINE static void DeserializeObject(void* obj, size_t hash);

    XNOR_ENGINE static void RegisterTypes();

    XNOR_ENGINE static std::string GetTypeName(size_t hash);

    template <typename T>
    static void FindAllChildClasses(std::vector<std::string>* names);

    XNOR_ENGINE static void Print();

private:
    XNOR_ENGINE static bool_t IsChildOf(size_t typeHash, size_t parentHash);

    XNOR_ENGINE static inline std::unordered_map<size_t, FactoryTypeInfo> m_FactoryMapHash;
    XNOR_ENGINE static inline std::unordered_map<std::string, FactoryTypeInfo> m_FactoryMapName;
};

END_XNOR_CORE

#include "reflection/xnor_factory.inl"
