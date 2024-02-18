#pragma once

#include "definitions.hpp"
#include "ImGui/imgui.h"
#include "utils/reflectable.hpp"

BEGIN_XNOR_EDITOR

class Inspector
{
public:
    void Display(XnorCore::Reflectable* obj);

private:
    void DisplayMember(void* obj, const XnorCore::FieldInfo& fieldInfo);
    void DisplayScalarMember(void* obj, const XnorCore::FieldInfo& fieldInfo, size_t element);

    template <typename T>
    static void DisplayScalar(void* obj, size_t offset, const char* name, size_t element);

    template <typename T>
    static constexpr T* GetPointer(void* obj, size_t offset, size_t element);
};

template <typename T>
void Inspector::DisplayScalar(void* const obj, const size_t offset, const char* const name, const size_t element)
{
    uint32_t type;

    if constexpr (std::is_same_v<T, int32_t>)
        type = ImGuiDataType_S32;
    else if constexpr (std::is_same_v<T, uint32_t>)
        type = ImGuiDataType_U32;
    else if constexpr (std::is_same_v<T, int16_t>)
        type = ImGuiDataType_S16;
    else if constexpr (std::is_same_v<T, uint16_t>)
        type = ImGuiDataType_U16;
    else if constexpr (std::is_same_v<T, int8_t>)
        type = ImGuiDataType_S8;
    else if constexpr (std::is_same_v<T, uint8_t>)
        type = ImGuiDataType_U8;
    else if constexpr (std::is_same_v<T, float_t>)
        type = ImGuiDataType_Float;
    else if constexpr (std::is_same_v<T, double_t>)
        type = ImGuiDataType_Double;

    ImGui::InputScalar(name, type, GetPointer<T>(obj, offset, element));
}

template <typename T>
constexpr T* Inspector::GetPointer(void* const obj, const size_t offset, const size_t element)
{
    uint8_t* const ptrSmall = static_cast<uint8_t*>(obj);
    void* const addr = ptrSmall + offset + sizeof(T) * element;
    return static_cast<T*>(addr);
}

END_XNOR_EDITOR
