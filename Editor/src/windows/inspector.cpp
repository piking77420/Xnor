#include "windows/inspector.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "Maths/quaternion.hpp"
#include "Maths/vector2.hpp"
#include "Maths/vector2i.hpp"
#include "Maths/vector3.hpp"
#include "Maths/vector4.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorEditor;

Inspector::Inspector(Editor* editor)
    : UiWindow(editor, "Inspector")
{
}

void Inspector::Display()
{
    void* const ptr = m_Editor->data.selectedEntity;
    ImGui::PushID(ptr);
    
    if (m_Editor->data.selectedEntity)
    {
	    const XnorCore::TypeInfo& info = XnorCore::TypeInfo::Get(typeid(*m_Editor->data.selectedEntity).hash_code());
        ImGui::Text("%s", info.GetName().c_str());
        
        for (const XnorCore::FieldInfo& m : info.GetMembers())
        {
            DisplayMember(ptr, m);
        }
    }

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
        DisplayArrayMember(obj, fieldInfo);        
        return;
    }

    if (fieldInfo.isVector)
    {
        DisplayVectorMember(obj, fieldInfo);
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
    else if (fieldInfo.typeHash == typeid(uint16_t).hash_code())
    {
        DisplayScalar<uint16_t>(obj, fieldInfo.offset, name, element);
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
    else if (fieldInfo.typeHash == typeid(bool).hash_code())
    {
        ImGui::Checkbox(name, XnorCore::Utils::GetAddress<bool>(obj, fieldInfo.offset, element));
    }
    else if (fieldInfo.typeHash == typeid(Vector2i).hash_code())
    {
        ImGui::DragInt2(name, XnorCore::Utils::GetAddress<Vector2i>(obj, fieldInfo.offset, element)->Raw());
    }
    else if (fieldInfo.typeHash == typeid(Vector2).hash_code())
    {
        ImGui::DragFloat2(name, XnorCore::Utils::GetAddress<Vector2>(obj, fieldInfo.offset, element)->Raw());
    }
    else if (fieldInfo.typeHash == typeid(Vector3).hash_code())
    {
        ImGui::DragFloat3(name, XnorCore::Utils::GetAddress<Vector3>(obj, fieldInfo.offset, element)->Raw());
    }
    else if (fieldInfo.typeHash == typeid(Vector4).hash_code())
    {
        ImGui::DragFloat4(name, XnorCore::Utils::GetAddress<Vector4>(obj, fieldInfo.offset, element)->Raw());
    }
    else if (fieldInfo.typeHash == typeid(Quaternion).hash_code())
    {
        Quaternion* const q = XnorCore::Utils::GetAddress<Quaternion>(obj, fieldInfo.offset, element);

        Vector3 euler = XnorCore::Utils::GetQuaternionEulerAngles(*q);
        euler *= Calc::Deg2Rad;
        ImGui::DragFloat3(name, euler.Raw());
        
        *q = Quaternion::FromEuler(euler);
    }
    else if (fieldInfo.typeHash == typeid(std::string).hash_code())
    {
        ImGui::InputText(name, XnorCore::Utils::GetAddress<std::string>(obj, fieldInfo.offset, element));
    }
    else
    {
        if (ImGui::CollapsingHeader(name))
        {
            size_t hash = fieldInfo.typeHash;
            const bool isPoly = fieldInfo.isPolyPointer; 

            PolyPtr<void*>* polyPtr = nullptr;
            if (isPoly)
            {
                polyPtr = XnorCore::Utils::GetAddress<PolyPtr<void*>>(obj, fieldInfo.offset, element); 
                hash = polyPtr->GetHash();
            }

            const XnorCore::TypeInfo& subInfo = XnorCore::TypeInfo::Get(hash);

            void* subPtr;
            if (isPoly)
                subPtr = polyPtr->AsVoid();
            else
                subPtr = XnorCore::Utils::GetAddress<uint8_t>(obj, fieldInfo.offset, element * subInfo.GetSize());
            
            ImGui::PushID(subPtr);

            for (const XnorCore::FieldInfo& m : subInfo.GetMembers())
                DisplayMember(subPtr, m);
            
            ImGui::PopID();
        }
    }
}

void Inspector::DisplayArrayMember(void* const obj, const XnorCore::FieldInfo& fieldInfo)
{
    if (ImGui::CollapsingHeader(fieldInfo.name.c_str()))
    {
        for (size_t i = 0; i < fieldInfo.GetArraySize(); i++)
            DisplayScalarMember(obj, fieldInfo, i);
    }
}

void Inspector::DisplayVectorMember(void* const obj, const XnorCore::FieldInfo& fieldInfo)
{
    if (ImGui::CollapsingHeader(fieldInfo.name.c_str()))
    {
        XnorCore::List<uint8_t>* const vec = XnorCore::Utils::GetAddress<XnorCore::List<uint8_t>>(obj, fieldInfo.offset, 0);

        if (!fieldInfo.isPolyPointer)
        {
            if (ImGui::Selectable("Add"))
            {
                vec->AddZeroed();
            }
        }
        
        void* ptr = vec->GetData() - fieldInfo.offset;

        size_t size = vec->GetSize();
        for (size_t i = 0; i < size; i++)
        {
            ImGui::PushID(static_cast<int32_t>(i));
            if (ImGui::Button("-"))
            {
                vec->RemoveAt(i); // TODO FIXME Memory leak
                ptr = vec->GetData() - fieldInfo.offset;
                size--;
            }
        
            ImGui::SameLine();
            
            DisplayScalarMember(ptr, fieldInfo, i);
            ImGui::PopID();
        }
    }
}
