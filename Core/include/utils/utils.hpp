// ReSharper disable CppInconsistentNaming
#pragma once

#include <filesystem>
#include <type_traits>
#include <vector>

#include "ImGui/imgui.h"

#include "core.hpp"
#include "Maths/vector2.hpp"

#include "utils/list.hpp"

BEGIN_XNOR_CORE

namespace Utils
{
    template<typename PtrT, typename IntT>
    [[nodiscard]]
    constexpr PtrT IntToPointer(IntT number);

    template <typename T>
    [[nodiscard]]
    constexpr T* GetAddress(const void* obj, size_t offset, size_t element);

    template<typename>
    struct is_std_vector : std::false_type {};

    template<typename T, typename A>
    struct is_std_vector<std::vector<T, A>> : std::true_type {};

    template<typename T>
    struct ptr_to_void_ptr { using type = T; };

    template<typename T>
    struct ptr_to_void_ptr<T*> { using type = void*; };

    XNOR_ENGINE void CenterImguiObject(float alignment = 0.5f);

    [[nodiscard]]
    XNOR_ENGINE ImVec2 ToImVec(Vector2 v);

    [[nodiscard]]
    XNOR_ENGINE Vector2 FromImVec(ImVec2 v);

    [[nodiscard]]
    XNOR_ENGINE std::string PathToForwardSlashes(const std::filesystem::path& path);

    [[nodiscard]]
    XNOR_ENGINE std::string PathToForwardSlashes(std::string path);

    [[nodiscard]]
    XNOR_ENGINE std::string HumanizeString(const std::string& str);
}

template<typename PtrT, typename IntT>
constexpr PtrT Utils::IntToPointer(const IntT number)
{
    static_assert(std::is_pointer_v<PtrT>); // PtrT must be a raw pointer type, ex: PtrT=int*
    static_assert(std::is_integral_v<IntT>); // IntT must be an integral type, ex: IntT=int
    
    return reinterpret_cast<const PtrT>(const_cast<uint8_t* const>(reinterpret_cast<const uint8_t* const>(1) + static_cast<const size_t>(number) - 1));
}

template <typename T>
constexpr T* Utils::GetAddress(const void* const obj, const size_t offset, const size_t element)
{
    return const_cast<T* const>(reinterpret_cast<const T* const>(static_cast<const uint8_t* const>(obj) + offset + sizeof(T) * element));
}

END_XNOR_CORE
