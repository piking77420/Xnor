#pragma once

// ReSharper disable CppInconsistentNaming

/// @file meta_programming.hpp
///
/// @brief Defines utilities for meta programming and template manipulation

#include <type_traits>

#include <Maths/quaternion.hpp>
#include <Maths/vector2.hpp>
#include <Maths/vector3.hpp>

#include "utils/color.hpp"
#include "utils/list.hpp"
#include "utils/pointer.hpp"
#include "utils/poly_ptr.hpp"

#include "core.hpp"

BEGIN_XNOR_CORE

/// @brief Defines utilities for meta programming and template manipulation
namespace Meta
{
    /// @brief Checks whether @c Derived is a derived class of @c Base
    /// @tparam Base Base class
    /// @tparam Derived Derived class
    template <typename Base, typename Derived>
    constexpr bool_t IsBaseOf = std::is_base_of_v<Base, Derived>;

    /// @brief Checks whether @c A and @c B are the same type
    /// @tparam A First type
    /// @tparam B Second type
    template <typename A, typename B>
    constexpr bool_t IsSame = std::is_same_v<A, B>;

    /// @brief Checks whether @c T is any of the provided types in @c Other
    /// @tparam T Type
    /// @tparam Other Other types
    template <typename T, typename... Other>
    constexpr bool_t IsAny = (std::is_same_v<T, Other> || ...);

    /// @brief Checks whether @c T is an array
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsArray = std::is_array_v<T>;

    /// @brief Checks whether @c T is a pointer
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsPointer = std::is_pointer_v<T>;

    /// @brief Checks whether @c T is an enum
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsEnum = std::is_enum_v<T>;

    /// @brief Checks whether @c T is an integral type
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsIntegral = std::is_integral_v<T>;

    /// @brief Checks whether @c T is a floating type
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsFloatingPoint = std::is_floating_point_v<T>;

    /// @brief Removes the array specification from @c T
    ///
    /// e.g. if @c T was @c int[4], then @c RemoveArraySpecifier<T> will be @c int
    ///
    template <typename T>
    using RemoveArraySpecifier = std::remove_extent_t<T>;

    /// @brief Removes the pointer specification from @c T
    ///
    /// e.g. if @c T was @c int*, then @c RemovePointerSpecifier<T> will be @c int
    ///
    template <typename T>
    using RemovePointerSpecifier = std::remove_pointer_t<T>;

    /// @brief Checks whether the type is a @c std::vector
    template <typename>
    constexpr bool_t IsStdVector = false;

    template <typename T, typename A>
    constexpr bool_t IsStdVector<std::vector<T, A>> = true;

    /// @brief Checks whether the type is a @ref List
    template <typename>
    constexpr bool_t IsXnorList = false;

    /// @private
    template <typename T>
    constexpr bool_t IsXnorList<List<T>> = true;

    /// @brief Checks whether the type is a @ref PolyPtr
    template <typename>
    constexpr bool_t IsPolyPtr = false;

    /// @private
    template <typename T>
    constexpr bool_t IsPolyPtr<PolyPtr<T>> = true;

    /// @brief Checks whether the type is a @ref Pointer
    template <typename>
    constexpr bool_t IsXnorPointer = false;

    /// @private
    template <typename T>
    constexpr bool_t IsXnorPointer<Pointer<T>> = true;

    /// @brief Checks if T is a native type.
    /// 
    /// A native type is one of the following types:
    /// - uint8_t
    /// - int8_t
    /// - uint16_t
    /// - int16_t
    /// - uint32_t
    /// - int32_t
    /// - float_t
    /// - double_t
    /// - bool_t_t
    /// 
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsNativeType = IsAny<T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, float_t, double_t, bool_t>;

    /// @brief Checks if T is an integral or a floating type.
    /// 
    /// A int/float type is one of the following types:
    /// - uint8_t
    /// - int8_t
    /// - uint16_t
    /// - int16_t
    /// - uint32_t
    /// - int32_t
    /// - float_t
    /// - double_t
    /// 
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsIntegralOrFloating = IsAny<T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, float_t, double_t>;

    /// @brief Checks if T is a math type.
    /// 
    /// A math type is one of the following types:
    /// - Vector2
    /// - Vector2i
    /// - Vector3
    /// - Vector4
    /// - Quaternion
    /// 
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsMathType = IsAny<T, Vector2, Vector2i, Vector3, Vector4, Quaternion>;
    
    /// @brief Checks if T is a color type.
    /// 
    /// A color type is one of the following types:
    /// - Colorf
    /// - ColorRgb
    /// - ColorHsv
    /// 
    /// @tparam T Type
    template <typename T>
    constexpr bool_t IsColorType = IsAny<T, Colorf, ColorRgb, ColorHsv>;
}

END_XNOR_CORE;
