#include "windows/inspector.hpp"

#include "imgui/imgui.h"
#include "Maths/vector2i.hpp"
#include "Maths/vector2.hpp"
#include "Maths/vector3.hpp"
#include "Maths/vector4.hpp"

using namespace XnorEditor;

void Inspector::Display(XnorCore::Reflectable* obj)
{
    void* const ptr = obj;
    ImGui::PushID(ptr);
	const XnorCore::TypeInfo& info = XnorCore::TypeInfo::Get(typeid(*obj).hash_code());

    ImGui::Begin(info.GetName().c_str());

    for (const XnorCore::FieldInfo& m : info.GetMembers())
    {
        DisplayMember(ptr, m);
    }

    ImGui::End();
    ImGui::PopID();
}

void Inspector::DisplayMember(void* obj, const XnorCore::FieldInfo& fieldInfo)
{
    if (fieldInfo.isPointer)
    {
        // TODO handle pointers
        return;
    }

    if (fieldInfo.isArray)
    {
        if (ImGui::CollapsingHeader(fieldInfo.name.c_str()))
        {
            for (size_t i = 0; i < fieldInfo.GetArraySize(); i++)
                DisplayScalarMember(obj, fieldInfo, i);
        }
        
        return;
    }

    DisplayScalarMember(obj, fieldInfo, std::numeric_limits<size_t>::max());
}

void Inspector::DisplayScalarMember(void* obj, const XnorCore::FieldInfo& fieldInfo, size_t element)
{
    const char* name;
    std::string nameStr;
    
    if (element == std::numeric_limits<size_t>::max())
    {
        name = fieldInfo.name.c_str();
        element = 0;
    }
    else
    {
        nameStr = std::string('[' + std::to_string(element) + ']');
        name = nameStr.c_str();
    }

 
    if (fieldInfo.typeHash == typeid(int32_t).hash_code())
    {
        DisplayScalar<int32_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(uint32_t).hash_code())
    {
        DisplayScalar<uint32_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(int16_t).hash_code())
    {
        DisplayScalar<int16_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(int16_t).hash_code())
    {
        DisplayScalar<int16_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(int8_t).hash_code())
    {
        DisplayScalar<int8_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(uint8_t).hash_code())
    {
        DisplayScalar<uint8_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(float_t).hash_code())
    {
        DisplayScalar<float_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(double_t).hash_code())
    {
        DisplayScalar<double_t>(obj, fieldInfo.offset, name, element);
    }
    else if (fieldInfo.typeHash == typeid(Vector2i).hash_code())
    {
        ImGui::InputInt2(name, GetPointer<Vector2i>(obj, fieldInfo.offset, element)->Raw());
    }
    else if (fieldInfo.typeHash == typeid(Vector2).hash_code())
    {
        ImGui::InputFloat2(name, GetPointer<Vector2>(obj, fieldInfo.offset, element)->Raw());
    }
    else if (fieldInfo.typeHash == typeid(Vector3).hash_code())
    {
        ImGui::InputFloat3(name, GetPointer<Vector3>(obj, fieldInfo.offset, element)->Raw());
    }
    else if (fieldInfo.typeHash == typeid(Vector4).hash_code())
    {
        ImGui::InputFloat4(name, GetPointer<Vector4>(obj, fieldInfo.offset, element)->Raw());
    }
    else
    {
        // TODO handle more complex types
    }
}
