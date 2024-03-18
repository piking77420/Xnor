#pragma once

#include "core.hpp"
#include "reflection/type_renderer.hpp"
#include "serialization/serializer.hpp"

#include <unordered_map>

BEGIN_XNOR_CORE

template <typename T>
constexpr void Factory::RegisterFactoryType()
{
    // ReSharper disable once CppTooWideScope
    constexpr bool_t isConstructible = !Meta::IsAbstract<T> && Meta::IsDefaultConstructible<T>;

    const size_t hash = Utils::GetTypeHash<T>();
    if (m_FactoryMapHash.contains(hash))
        return;

    constexpr TypeDescriptor<T> desc = Reflection::GetTypeInfo<T>();

    const constexpr char_t* const name = desc.name.c_str();
    /* constexpr */ const char_t* const humanizedName = Utils::RemoveNamespaces(name);
    
    FactoryTypeInfo info = {
        .displayFunc = [](void* const obj) -> void { TypeRenderer::DisplayObject<T>(static_cast<T*>(obj)); },
        .serializeFunc = [](void* const obj) -> void { Serializer::Serialize<T, false>(static_cast<T*>(obj)); },
        .deserializeFunc = [](void* const obj) -> void { Serializer::Deserialize<T, false>(static_cast<T*>(obj)); },
        .isConstructible = isConstructible,
        .name = humanizedName
    };

    if constexpr (isConstructible)
        info.createFunc = []() -> void* { return new T(); };
    else
        info.createFunc = []() -> void* { return nullptr; };

    refl::util::for_each(refl::util::reflect_types(desc.declared_bases), [&]<typename ParentT>(const ParentT)
    {
        info.parentClasses.push_back(Utils::GetTypeHash<typename ParentT::type>());
    });

    m_FactoryMapHash.emplace(hash, info);
    m_FactoryMapName.emplace(humanizedName, info);
}

inline bool_t Factory::IsChildOf(const size_t typeHash, const size_t parentHash)
{
    auto&& it = m_FactoryMapHash.find(typeHash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find child type : {}", typeHash);
        return false;
    }

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
