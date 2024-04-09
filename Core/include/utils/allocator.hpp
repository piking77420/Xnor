#pragma once

#include "core.hpp"

/// @file allocator.hpp
/// @brief Defines the XnorCore::Allocator class.

BEGIN_XNOR_CORE

class Allocator
{
    STATIC_CLASS(Allocator)
    
public:
    template <typename T>
    ALLOCATOR_FUNC static T* Allocate(size_t count);
    
    template <typename T, typename... Args>
    ALLOCATOR_FUNC static void Construct(T* where, Args&&... args);
    
    template <typename T>
    static void Deallocate(T* where, size_t count);

private:
    template <typename T>
    [[nodiscard]]
    static constexpr size_t GetAllocationSize(size_t count);

    template <typename T>
    [[nodiscard]]
    static constexpr std::align_val_t GetAlignment();
};

END_XNOR_CORE

#include "utils/allocator.inl"
