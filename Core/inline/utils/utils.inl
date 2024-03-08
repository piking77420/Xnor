#pragma once

#include <type_traits>

#include "utils/list.hpp"

BEGIN_XNOR_CORE

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

template <typename T>
size_t Utils::GetTypeHash([[maybe_unused]] const T* const ptr)
{
    return typeid(*ptr).hash_code();
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

template <std::ranges::input_range Container, typename T>
bool_t Utils::ArrayContains(Container container, T element)
{
    return std::ranges::find(container, element) != container.end();
}

template <std::ranges::input_range Container>
bool_t Utils::StringArrayContains(Container container, const std::string& element)
{
    for (const std::string& elem : container)
    {
        if (StringEqualsIgnoreCase(elem, element))
            return true;
    }

    return false;
}

END_XNOR_CORE
