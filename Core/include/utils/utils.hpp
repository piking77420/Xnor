#pragma once

#include <type_traits>

#include "core.hpp"

BEGIN_XNOR_CORE

namespace Utils
{
    template<typename PtrT, typename IntT>
    constexpr PtrT IntToPointer(IntT number);

    template <typename T>
    constexpr T* GetObjectPointer(const void* obj, size_t offset, size_t element);
}

template<typename PtrT, typename IntT>
constexpr PtrT Utils::IntToPointer(const IntT number)
{
    static_assert(std::is_pointer_v<PtrT>);
    static_assert(std::is_integral_v<IntT>);
    
    return reinterpret_cast<const PtrT>(const_cast<uint8_t* const>(reinterpret_cast<const uint8_t* const>(1) + static_cast<const size_t>(number) - 1));
}

template <typename T>
constexpr T* Utils::GetObjectPointer(const void* const obj, const size_t offset, const size_t element)
{
    return const_cast<T* const>(reinterpret_cast<const T* const>(static_cast<const uint8_t* const>(obj) + offset + sizeof(T) * element));
}

END_XNOR_CORE
