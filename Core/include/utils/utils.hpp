// ReSharper disable CppInconsistentNaming
#pragma once

#include <filesystem>
#include <type_traits>
#include <vector>

#include <Maths/quaternion.hpp>
#include <Maths/vector2.hpp>
#include <Maths/vector3.hpp>

#include <ImGui/imgui.h>

#include "core.hpp"

#include "utils/color.hpp"
#include "utils/list.hpp"
#include "utils/pointer.hpp"
#include "utils/poly_ptr.hpp"

BEGIN_XNOR_CORE

class File;
class Directory;
class Entry;

namespace Utils
{
    template<typename PtrT, typename IntT>
    [[nodiscard]]
    constexpr PtrT IntToPointer(IntT number);

    template <typename T>
    [[nodiscard]]
    constexpr T* GetAddress(const void* obj, size_t offset, size_t element);

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

    /// @brief Checks if T is a math type.
    /// 
    /// A native type is one of the following types:
    /// - Vector2 \n
    /// - Vector2i \n
    /// - Vector3 \n
    /// - Vector4 \n
    /// - Quaternion
    /// 
    /// @tparam T Type
    /// @return Whether @p T is a native type
    template<typename T>
    constexpr bool IsMathType = std::is_same_v<T, Vector2> || std::is_same_v<T, Vector2i> || std::is_same_v<T, Vector3> ||
                                std::is_same_v<T, Vector4> || std::is_same_v<T, Quaternion>;
    
    XNOR_ENGINE void AlignImGuiCursor(float_t objectWidth, float_t alignment = 0.5f);

    [[nodiscard]]
    XNOR_ENGINE ImVec2 ToImVec(Vector2 v);

    [[nodiscard]]
    XNOR_ENGINE Vector2 FromImVec(ImVec2 v);

    [[nodiscard]]
    XNOR_ENGINE ImVec4 ToImCol(Color color);

    [[nodiscard]]
    XNOR_ENGINE ImVec4 ToImCol(const Colorf& color);

    [[nodiscard]]
    XNOR_ENGINE ImVec4 ToImCol(ColorHsv color);

    [[nodiscard]]
    XNOR_ENGINE Colorf FromImCol(const ImVec4& color);

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

    XNOR_ENGINE void OpenInExplorer(const Entry& entry);

    XNOR_ENGINE void OpenInExplorer(const Directory& directory);

    XNOR_ENGINE void OpenInExplorer(const File& file);

    XNOR_ENGINE void OpenInExplorer(const std::filesystem::path& path);

    XNOR_ENGINE void OpenInExplorer(const std::filesystem::path& path, bool isFile);

    XNOR_ENGINE void OpenFile(const File& file);

    XNOR_ENGINE void OpenFile(const std::filesystem::path& filepath);
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
