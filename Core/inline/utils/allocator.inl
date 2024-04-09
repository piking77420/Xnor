#pragma once

#include <new>

BEGIN_XNOR_CORE

template <typename T>
T* Allocator::Allocate(const size_t count)
{
    const size_t size = GetAllocationSize<T>(count);

    if (size == 0)
        return nullptr;
    
    return static_cast<T*>(::operator new(size, GetAlignment<T>()));
}

template <typename T, typename... Args>
void Allocator::Construct(T* const where, Args&&... args)
{
    ::new(static_cast<void*>(where)) T(std::forward<Args>(args)...);
}

template <typename T>
void Allocator::Deallocate(T* const where, const size_t count)
{
    const size_t size = GetAllocationSize<T>(count);
    ::operator delete(where, size, GetAlignment<T>());
}

template <typename T>
constexpr size_t Allocator::GetAllocationSize(const size_t count)
{
    constexpr size_t typeSize = sizeof(T);
    
    if constexpr (typeSize > 1)
    {
        constexpr size_t maxPossible = static_cast<size_t>(-1) / typeSize;
        if (count > maxPossible)
            throw std::bad_array_new_length{};
    }

    return count * typeSize;
}

template <typename T>
constexpr std::align_val_t Allocator::GetAlignment()
{
    return std::align_val_t{std::max(alignof(T), static_cast<size_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__))};
}

END_XNOR_CORE
