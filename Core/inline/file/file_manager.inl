#pragma once

#include <filesystem>

BEGIN_XNOR_CORE

template <EntryT T>
Pointer<T> FileManager::Get(const std::filesystem::path& path)
{
    if (!Contains(path))
    {
        Logger::LogError("Attempt to get an unknown FileManager entry: {}", path);
        return nullptr;
    }

    return static_cast<Pointer<T>>(m_Entries.at(path));
}

template <EntryT T>
Pointer<T> FileManager::Find()
{
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<Entry> entry = mapEntry.second;
        
        if (Utils::DynamicPointerCast<T>(entry))
            return entry;
    }

    return nullptr;
}

template <EntryT T>
Pointer<T> FileManager::Find(std::function<bool_t(Pointer<T>)>&& predicate)
{
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<Entry> entry = mapEntry.second;
        
        if (Utils::DynamicPointerCast<T>(entry) && predicate(entry))
            return entry;
    }

    return nullptr;
}

template <EntryT T>
std::vector<Pointer<T>> FileManager::FindAll()
{
    std::vector<Pointer<T>> result;
    FindAll<T>(&result);
    return result;
}

template <EntryT T>
void FileManager::FindAll(std::vector<Pointer<T>>* result)
{
    result->clear();
    
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<T> entry = Utils::DynamicPointerCast<T>(mapEntry.second);
        
        if (entry)
            result->push_back(std::move(entry));
    }
}

template <EntryT T>
std::vector<Pointer<T>> FileManager::FindAll(std::function<bool_t(Pointer<T>)>&& predicate)
{
    std::vector<Pointer<T>> result;
    FindAll<T>(predicate, &result);
    return result;
}

template <EntryT T>
void FileManager::FindAll(std::function<bool(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result)
{
    result->clear();
    
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<Entry> entry = mapEntry.second;
        
        if (Utils::DynamicPointerCast<T>(entry) && predicate(entry))
            result->push_back(entry);
    }
}

END_XNOR_CORE
