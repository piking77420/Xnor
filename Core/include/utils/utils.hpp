// ReSharper disable CppInconsistentNaming
#pragma once

#include <filesystem>
#include <type_traits>
#include <vector>
#include <Maths/quaternion.hpp>
#include <Maths/vector3.hpp>
#include <Maths/vector4.hpp>

#include "ImGui/imgui.h"

#include "core.hpp"
#include "Maths/vector2.hpp"

#include "utils/list.hpp"
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

    XNOR_ENGINE void CenterImguiObject(float alignment = 0.5f);

    [[nodiscard]]
    XNOR_ENGINE ImVec2 ToImVec(Vector2 v);

    [[nodiscard]]
    XNOR_ENGINE Vector2 FromImVec(ImVec2 v);

    [[nodiscard]]
    XNOR_ENGINE std::string HumanizeString(const std::string& str);

    XNOR_ENGINE inline float NormalizeAngle(float angle)
    {
        while (angle > Calc::Pi * 2.f)
            angle -= Calc::Pi * 2.f;
        
        while (angle < 0)
            angle += Calc::Pi * 2.f;
        
        return angle;
    }
    
    XNOR_ENGINE inline Vector3 NormalizeAngles(Vector3 angles)
    {
        angles.x = NormalizeAngle(angles.x);
        angles.y = NormalizeAngle(angles.y);
        angles.z = NormalizeAngle(angles.z);
        return angles;
    }

    XNOR_ENGINE inline Vector3 GetQuaternionEulerAngles(Quaternion rot)
    {
        float sqw = rot.W() * rot.W();
        float sqx = rot.X() * rot.X();
        float sqy = rot.Y() * rot.Y();
        float sqz = rot.Z() * rot.Z();
        float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
        float test = rot.X() * rot.W() - rot.Y() * rot.Z();
        Vector3 v;
 
        if (test > 0.4995f * unit)
        { // singularity at north pole
            v.y = 2.f * std::atan2f(rot.Y(), rot.X());
            v.x = Calc::PiOver2;
            v.z = 0;
            return NormalizeAngles(v);
        }
        if (test < -0.4995f * unit)
        { // singularity at south pole
            v.y = -2.f * std::atan2f(rot.Y(), rot.X());
            v.x = -Calc::PiOver2;
            v.z = 0;
            return NormalizeAngles(v);
        }
 
        rot = Quaternion(rot.Z(), rot.X(), rot.Y(), rot.W());
        v.y = std::atan2f(2.f * rot.X() * rot.W() + 2.f * rot.Y() * rot.Z(), 1 - 2.f * (rot.Z() * rot.Z() + rot.W() * rot.W()));     // Yaw
        v.x = std::asinf(2.f * (rot.X() * rot.Z() - rot.W() * rot.Y()));                             // Pitch
        v.z = std::atan2f(2.f * rot.X() * rot.Y() + 2.f * rot.Z() * rot.W(), 1 - 2.f * (rot.Y() * rot.Y() + rot.Z() * rot.Z()));      // Roll
        return NormalizeAngles(v);
    }
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
