#pragma once

#include "utils/factory.hpp"

#include "core.hpp"
#include "serialization/serializer.hpp"

#include <unordered_map>

BEGIN_XNOR_CORE

template <typename T>
constexpr void Factory::RegisterFactoryType()
{
    constexpr TypeDescriptor<T> desc = Reflection::GetTypeInfo<T>();
    const constexpr char_t* const name = desc.name.c_str();

    FactoryTypeInfo info = {
        .createFunc = []() -> void* { return new T(); },
        .displayFunc = [](void* const) -> void { /* XnorEditor::Inspector::DisplayObject<T>(static_cast<T*>(obj)); */ },
        .serializeFunc = [](void* const obj) -> void { Serializer::Serialize<T>(static_cast<T*>(obj), false); },
        .name = name
    };

    refl::util::for_each(refl::util::reflect_types(desc.declared_bases), [&]<typename ParentT>(const ParentT)
    {
        info.parentClasses.push_back(Utils::GetTypeHash<typename ParentT::type>());
    });

    m_FactoryMapHash.emplace(Utils::GetTypeHash<T>(), info);
    m_FactoryMapName.emplace(name, info);
}

inline void* Factory::CreateObject(const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return nullptr;
    }

    return it->second.createFunc();
}

inline void* Factory::CreateObject(const char_t* const name)
{
    auto&& it = m_FactoryMapName.find(name);

    if (it == m_FactoryMapName.end())
    {
        Logger::LogError("Couldn't find type : {}", name);
        return nullptr;
    }

    return it->second.createFunc();
}

inline void Factory::DisplayObject(void* const obj, const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return;
    }

    it->second.displayFunc(obj);
}

inline bool_t Factory::IsChildOf(const size_t typeHash, const size_t parentHash)
{
    auto&& it = m_FactoryMapHash.find(typeHash);

    const std::vector<size_t>& parents = it->second.parentClasses;
    
    auto&& parentIt = std::ranges::find(parents, parentHash);

    if (parentIt != parents.end())
        return true;

    for (size_t i = 0; i < parents.size(); i++)
    {
        if (IsChildOf(parents[i], parentHash))
            return true;
    }

    return false;
}

template <typename T>
void Factory::FindAllChildClasses(std::vector<const char_t*>* names)
{
    const size_t hash = Utils::GetTypeHash<T>();

    for (auto&& it : m_FactoryMapHash)
    {
        if (IsChildOf(it.first, hash))
            names->push_back(it.second.name);
    }
}

END_XNOR_CORE
