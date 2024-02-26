#pragma once

#include <tuple>
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

namespace Meta
{
    template<typename>
    constexpr bool IsStdVector = false;

    template<typename T, typename A>
    constexpr bool IsStdVector<std::vector<T, A>> = true;

    template<typename>
    constexpr bool IsXnorVector = false;

    template<typename T>
    constexpr bool IsXnorVector<List<T>> = true;

    template<typename>
    constexpr bool IsPolyPtr = false;

    template<typename T>
    constexpr bool IsPolyPtr<PolyPtr<T>> = true;

    template<typename>
    constexpr bool IsXnorPointer = false;

    template<typename T>
    constexpr bool IsXnorPointer<Pointer<T>> = true;

    /// @brief Checks if T is a native type.
    /// 
    /// A native type is one of the following types:
    /// - uint8_t \n
    /// - int8_t \n
    /// - uint16_t \n
    /// - int16_t \n
    /// - uint32_t \n
    /// - int32_t \n
    /// - float_t \n
    /// - double_t \n
    /// - bool_t
    /// 
    /// @tparam T Type
    /// @return Whether @p T is a native type.
    template<typename T>
    constexpr bool IsNativeType = std::is_same_v<T, uint8_t> || std::is_same_v<T, int8_t> || std::is_same_v<T, uint16_t> ||
                                    std::is_same_v<T, int16_t> || std::is_same_v<T, uint32_t> || std::is_same_v<T, int32_t> ||
                                    std::is_same_v<T, float_t> || std::is_same_v<T, double_t> || std::is_same_v<T, bool_t>;

    /// @brief Checks if T is an integral or a floating type.
    /// 
    /// A int/float type is one of the following types:
    /// - uint8_t \n
    /// - int8_t \n
    /// - uint16_t \n
    /// - int16_t \n
    /// - uint32_t \n
    /// - int32_t \n
    /// - float_t \n
    /// - double_t
    /// 
    /// @tparam T Type
    /// @return Whether @p T is an integral or a floating type.
    template<typename T>
    constexpr bool IsIntegralOrFloating = std::is_same_v<T, uint8_t> || std::is_same_v<T, int8_t> || std::is_same_v<T, uint16_t> ||
                                    std::is_same_v<T, int16_t> || std::is_same_v<T, uint32_t> || std::is_same_v<T, int32_t> ||
                                    std::is_same_v<T, float_t> || std::is_same_v<T, double_t>;

    /// @brief Checks if T is a math type.
    /// 
    /// A math type is one of the following types:
    /// - Vector2 \n
    /// - Vector2i \n
    /// - Vector3 \n
    /// - Vector4 \n
    /// - Quaternion
    /// 
    /// @tparam T Type
    /// @return Whether @p T is a math type
    template<typename T>
    constexpr bool IsMathType = std::is_same_v<T, Vector2> || std::is_same_v<T, Vector2i> || std::is_same_v<T, Vector3> ||
                                std::is_same_v<T, Vector4> || std::is_same_v<T, Quaternion>;
    
    /// @brief Checks if T is a color type.
    /// 
    /// A color type is one of the following types:
    /// - Colorf \n
    /// - ColorRgb \n
    /// - ColorHsv
    /// 
    /// @tparam T Type
    /// @return Whether @p T is a color type
    template<typename T>
    constexpr bool IsColorType = std::is_same_v<T, Colorf> || std::is_same_v<T, ColorRgb> || std::is_same_v<T, ColorHsv>;
}

END_XNOR_CORE;
