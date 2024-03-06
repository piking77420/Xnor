﻿// ReSharper disable CppInconsistentNaming
#pragma once

#include <filesystem>
#include <type_traits>

#include <Maths/quaternion.hpp>
#include <Maths/vector2.hpp>
#include <Maths/vector3.hpp>

#include <ImGui/imgui.h>

#include "core.hpp"

#include "utils/color.hpp"
#include "utils/list.hpp"
#include "utils/pointer.hpp"

/// @file utils.hpp

BEGIN_XNOR_CORE

class File;
class Directory;
class Entry;

/// @brief Namespace containing utility functions that don't belong anywhere else
namespace Utils
{
    /// @brief Converts a integral number to a valid pointer without illegal size operations
    /// @tparam PtrT Type of the pointer, must be a pointer type
    /// @tparam IntT Type of the number, must be integral
    /// @param number Number to convert
    /// @return Pointer representation of the number
    template <typename PtrT, typename IntT>
    [[nodiscard]]
    constexpr PtrT IntToPointer(IntT number);

    /// @brief Gets the hash code of a specified type
    /// @tparam T Type
    /// @return Hash
    template <typename T>
    [[nodiscard]]
    size_t GetTypeHash();

    /// @brief Horizontally aligns the cursor of ImGui to be centered around a specific portion of the available space
    /// @param objectWidth Width of the element to align
    /// @param alignment In window alignment, 0.5f by default to center the object
    XNOR_ENGINE void AlignImGuiCursor(float_t objectWidth, float_t alignment = 0.5f);

    /// @brief Converts a Vector2 to an ImVec2
    /// @param v Vector2 to convert
    /// @return ImVec2 representation
    [[nodiscard]]
    XNOR_ENGINE constexpr ImVec2 ToImVec(Vector2 v);

    /// @brief Converts an ImVec2 to a Vector2
    /// @param v ImVec2 to convert
    /// @return Vector2 representation
    [[nodiscard]]
    XNOR_ENGINE constexpr Vector2 FromImVec(ImVec2 v);

    /// @brief Converts a @ref ColorRgb to an ImVec4
    /// @param color ColorRgb to convert
    /// @return ImVec4 representation
    [[nodiscard]]
    XNOR_ENGINE constexpr ImVec4 ToImCol(ColorRgb color);

    /// @brief Converts a @ref Colorf to an ImVec4
    /// @param color Colorf to convert
    /// @return ImVec4 representation
    [[nodiscard]]
    XNOR_ENGINE constexpr ImVec4 ToImCol(const Colorf& color);

    /// @brief Converts a @ref ColorHsv to an ImVec4
    /// @param color ColorHsv to convert
    /// @return ImVec4 representation
    [[nodiscard]]
    XNOR_ENGINE constexpr ImVec4 ToImCol(ColorHsv color);

    /// @brief Converts an ImVec4 to a @ref Colorf
    /// @param color ImVec4 to convert
    /// @return Colorf representation
    [[nodiscard]]
    XNOR_ENGINE constexpr Colorf FromImCol(const ImVec4& color);

    /// @brief Humanizes the provided string
    ///
    /// The process converts a PascalCase styled word to a humanized version that puts spaces between each words and adds an uppercase at the very beginning
    ///
    /// e.g. RequiresUIReloadAttribute will become Requires UI Reload Attribute
    /// @param str String to humanize
    /// @return Result
    [[nodiscard]]
    XNOR_ENGINE std::string HumanizeString(const std::string& str);

    /// @brief Normalizes an angle (clamps its value between 0 and 2 * PI)
    /// @param angle Angle to normalize
    /// @return Normalized representation
    XNOR_ENGINE float_t NormalizeAngle(float_t angle);

    /// @brief Normalizes a set of 3 angles in a Vector3 (clamps their value between 0 and 2 * PI)
    /// @param angles Vector3 of angles to normalize
    /// @return Normalized representation
    XNOR_ENGINE Vector3 NormalizeAngles(Vector3 angles);

    /// @brief Converts a quaternion to its euler angle representation
    /// @param rot Quaternion to convert
    /// @return Euler representation
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
    template <typename T, typename U>
    [[nodiscard]]
    Pointer<T> DynamicPointerCast(const Pointer<U>& value);

    /// @brief Opens the specified entry in the file explorer
    /// @param entry Entry
    XNOR_ENGINE void OpenInExplorer(const Entry& entry);

    /// @brief Opens the specified directory in the file explorer
    /// @param directory Directory
    XNOR_ENGINE void OpenInExplorer(const Directory& directory);

    /// @brief Opens the specified file in the file explorer
    /// @param file File
    XNOR_ENGINE void OpenInExplorer(const File& file);

    /// @brief Opens the specified path in the file explorer
    /// @param path File system path
    XNOR_ENGINE void OpenInExplorer(const std::filesystem::path& path);

    /// @brief Opens the specified path in the file explorer
    /// @param path File system path
    /// @param isFile Whether path is a file or a directory
    XNOR_ENGINE void OpenInExplorer(const std::filesystem::path& path, bool isFile);

    /// @brief Opens the specified file on the user's computer
    /// @param file File
    XNOR_ENGINE void OpenFile(const File& file);

    /// @brief Opens the specified file on the user's computer
    /// @param filepath File system path
    XNOR_ENGINE void OpenFile(const std::filesystem::path& filepath);
}

template <typename PtrT, typename IntT>
constexpr PtrT Utils::IntToPointer(const IntT number)
{
    static_assert(std::is_pointer_v<PtrT>, "PtrT must be a raw pointer type, ex: PtrT=int*");
    static_assert(std::is_integral_v<IntT>, "IntT must be an integral type, ex: IntT=int");
    
    return reinterpret_cast<PtrT>(reinterpret_cast<uint8_t*>(1) + static_cast<const size_t>(number) - 1);
}

constexpr ImVec2 Utils::ToImVec(const Vector2 v)
{
    return ImVec2(v.x, v.y);
}

constexpr Vector2 Utils::FromImVec(const ImVec2 v)
{
    return Vector2(v.x, v.y);
}

constexpr ImVec4 Utils::ToImCol(const ColorRgb color)
{
    const Colorf c = static_cast<Colorf>(color);
    return ImVec4(c.r, c.g, c.b, c.a);
}

constexpr ImVec4 Utils::ToImCol(const Colorf& color)
{
    return ImVec4(color.r, color.g, color.b, color.a);
}

constexpr ImVec4 Utils::ToImCol(const ColorHsv color)
{
    const Colorf c = static_cast<Colorf>(static_cast<ColorRgb>(color));
    return ImVec4(c.r, c.g, c.b, c.a);
}

constexpr Colorf Utils::FromImCol(const ImVec4& color)
{
    return Colorf(color.x, color.y, color.z, color.w);
}

template <typename T>
size_t Utils::GetTypeHash()
{
    return typeid(T).hash_code();
}

template <typename T, typename U>
Pointer<T> Utils::DynamicPointerCast(const Pointer<U>& value)
{
    if (!value)
        return nullptr;

    if (dynamic_cast<T*>(const_cast<U*>(static_cast<const U*>(value))))
        return Pointer<T>(value, value.GetIsStrongReference());

    return nullptr;
}

END_XNOR_CORE
