// ReSharper disable CppInconsistentNaming
#pragma once

#include <filesystem>
#include <type_traits>
#include <vector>
#include <Maths/quaternion.hpp>
#include <Maths/vector3.hpp>

#include "ImGui/imgui.h"

#include "core.hpp"
#include "Maths/vector2.hpp"

#include "utils/list.hpp"
#include "utils/pointer.hpp"
#include "utils/poly_ptr.hpp"

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

    template<typename>
    struct is_xnor_vector : std::false_type {};

    template<typename T>
    struct is_xnor_vector<List<T>> : std::true_type {};

    template<typename>
    struct is_poly_ptr : std::false_type {};

    template<typename T>
    struct is_poly_ptr<PolyPtr<T>> : std::true_type {};

    template<typename T>
    struct ptr_to_void_ptr { using type = T; };

    template<typename T>
    struct ptr_to_void_ptr<T*> { using type = void*; };

    /// @brief Checks if T is a native type \n
    /// A native type is one of the following types:
    /// - uint8_t \n
    /// - int8_t \n
    /// - uint16_t \n
    /// - int16_t \n
    /// - uint32_t \n
    /// - int32_t \n
    /// - float_t \n
    /// - double_t \n
    /// - bool_t \n
    /// @tparam T Type
    /// @return Is native type
    template <typename T>
    [[nodiscard]]
    constexpr bool IsNativeType();
    
    /// @brief Checks if T is a math type \n
    /// A native type is one of the following types:
    /// - Vector2 \n
    /// - Vector2i \n
    /// - Vector3 \n
    /// - Vector4 \n
    /// - Quaternion \n
    /// @tparam T Type
    /// @return Is native type
    template <typename T>
    [[nodiscard]]
    constexpr bool IsMathType(); 

    XNOR_ENGINE void CenterImguiObject(float_t alignment = 0.5f);

    [[nodiscard]]
    XNOR_ENGINE ImVec2 ToImVec(Vector2 v);

    [[nodiscard]]
    XNOR_ENGINE Vector2 FromImVec(ImVec2 v);

    [[nodiscard]]
    XNOR_ENGINE std::string HumanizeString(const std::string& str);

    XNOR_ENGINE float_t NormalizeAngle(float_t angle);

    XNOR_ENGINE Vector3 NormalizeAngles(Vector3 angles);

    XNOR_ENGINE Vector3 GetQuaternionEulerAngles(const Quaternion& rot);

    /// @brief Equivalent of a @c dynamic_cast for @ref Pointer "Pointers".
    ///
    /// This function first checks if the given @ref Pointer is null, and returns null if so.
    /// It then checks if one type is derived from the other, and returns the casted result if so. This actually performs a @c reinterpret_cast under the hood.
    /// If all other conditions failed, the function returns a null @ref Pointer.
    /// 
    /// @tparam T The type to dynamically cast to.
    /// @tparam U The type to dynamically cast from.
    /// @param value The @ref Pointer to cast from.
    /// @return A null @ref Pointer if the cast failed. Otherwise, the cast result.
    template<typename T, typename U>
    [[nodiscard]]
    Pointer<T> DynamicPointerCast(const Pointer<U>& value);
}

template<typename PtrT, typename IntT>
constexpr PtrT Utils::IntToPointer(const IntT number)
{
    static_assert(std::is_pointer_v<PtrT>, "PtrT must be a raw pointer type, ex: PtrT=int*");
    static_assert(std::is_integral_v<IntT>, "IntT must be an integral type, ex: IntT=int");
    
    return reinterpret_cast<const PtrT>(const_cast<uint8_t* const>(reinterpret_cast<const uint8_t* const>(1) + static_cast<const size_t>(number) - 1));
}

template <typename T>
constexpr T* Utils::GetAddress(const void* const obj, const size_t offset, const size_t element)
{
    return const_cast<T* const>(reinterpret_cast<const T* const>(static_cast<const uint8_t* const>(obj) + offset + sizeof(T) * element));
}

template <typename T>
constexpr bool Utils::IsNativeType()
{
    return (std::is_same_v<T, uint8_t> ||
        std::is_same_v<T, int8_t> ||
        std::is_same_v<T, uint16_t> ||
        std::is_same_v<T, int16_t> ||
        std::is_same_v<T, uint32_t> ||
        std::is_same_v<T, int32_t> ||
        std::is_same_v<T, float_t> ||
        std::is_same_v<T, double_t> ||
        std::is_same_v<T, bool_t>
    );
}

template <typename T>
constexpr bool Utils::IsMathType()
{
    return (std::is_same_v<T, Vector2> ||
        std::is_same_v<T, Vector2i> ||
        std::is_same_v<T, Vector3> ||
        std::is_same_v<T, Vector4> ||
        std::is_same_v<T, Quaternion>);
}

template<typename T, typename U>
Pointer<T> Utils::DynamicPointerCast(const Pointer<U>& value)
{
    if (!value)
        return nullptr;

    if (dynamic_cast<T*>(static_cast<U*>(value)))
        return Pointer<T>(value, value.GetIsStrongReference());

    return nullptr;
}

END_XNOR_CORE
