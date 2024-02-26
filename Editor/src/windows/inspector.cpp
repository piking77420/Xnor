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
    // Check if an entity was selected
    if (!m_Editor->data.selectedEntity)
    {
        ImGui::Text("Nothing selected");
        return;
    }
    
    // Get the base address of the selected entity, we'll use this pointer to do arithmetic on it to get the address of the different fields
    void* const ptr = m_Editor->data.selectedEntity;
    
    ImGui::PushID(ptr);

    // Request the type info of the object
	const XnorCore::TypeInfo& info = XnorCore::TypeInfo::Get(typeid(*m_Editor->data.selectedEntity).hash_code());
    ImGui::Text("%s", info.GetName().c_str());

    // Display each member
    for (const XnorCore::FieldInfo& m : info.GetMembers())
        DisplayMember(ptr, m);

    ImGui::PopID();
}

void Inspector::DisplayMember(void* obj, const XnorCore::FieldInfo& fieldInfo)
{
    if (fieldInfo.isPointer)
    {
        // TODO handle pointers
        return;
    }

    if (fieldInfo.isXnorPointer)
    {
        DisplayXnorPointerMember(obj, fieldInfo);
        return;
    }

    if (fieldInfo.isArray)
    {
        DisplayArrayMember(obj, fieldInfo);        
        return;
    }

    if (fieldInfo.isList)
    {
        DisplayVectorMember(obj, fieldInfo);
        return;
    }

    // It's just a normal member, display as scalar, use the max value to indicate we're not in an array
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

    if (fieldInfo.isNativeType)
    {
        DisplayNativeType(obj, fieldInfo, name, element);
        return;
    }

    if (fieldInfo.isMathType)
    {
        DisplayMathType(obj, fieldInfo, name, element);
        return;
    }

    if (fieldInfo.typeHash == typeid(std::string).hash_code())
    {
        ImGui::InputText(name, XnorCore::Utils::GetAddress<std::string>(obj, fieldInfo.offset, element));
        return;
    }

    if (fieldInfo.typeHash == typeid(XnorCore::ColorRgb).hash_code())
    {
        XnorCore::ColorRgb* const ptr = XnorCore::Utils::GetAddress<XnorCore::ColorRgb>(obj, fieldInfo.offset, element);
        XnorCore::Colorf tmp = static_cast<XnorCore::Colorf>(*ptr);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHex);
        *ptr = static_cast<XnorCore::ColorRgb>(tmp);
    }
    else if (fieldInfo.typeHash == typeid(XnorCore::ColorHsv).hash_code())
    {
        XnorCore::ColorHsv* const ptr = XnorCore::Utils::GetAddress<XnorCore::ColorHsv>(obj, fieldInfo.offset, element);
        XnorCore::Colorf tmp = static_cast<XnorCore::Colorf>(*ptr);
        ImGui::ColorPicker4(name, &tmp.r, ImGuiColorEditFlags_DisplayHSV);
        *ptr = static_cast<XnorCore::ColorHsv>(tmp);
    }
    else if (fieldInfo.typeHash == typeid(XnorCore::Colorf).hash_code())
    {
        ImGui::ColorPicker4(name, reinterpret_cast<float_t*>(XnorCore::Utils::GetAddress<XnorCore::Colorf>(obj, fieldInfo.offset, element)));
    }

    DisplayNestedType(obj, fieldInfo, name, element);
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
                vec->RemoveAt(i);
                ptr = vec->GetData() - fieldInfo.offset;
                size--;
            }
        
            ImGui::SameLine();
            
            if (!fieldInfo.isPolyPointer && ImGui::Button("+"))
            {
                vec->InsertZeroed(i);
                ptr = vec->GetData() - fieldInfo.offset;
                size++;
            }
        
            ImGui::SameLine();
            
            DisplayScalarMember(ptr, fieldInfo, i);
            ImGui::PopID();
        }
    }
}

void Inspector::DisplayXnorPointerMember(const void* obj, const XnorCore::FieldInfo& fieldInfo)
{
    // TODO support anything other than resource
    XnorCore::Pointer<XnorCore::Resource>* ptr = XnorCore::Utils::GetAddress<XnorCore::Pointer<XnorCore::Resource>>(obj, fieldInfo.offset, 0);

    ImGui::Text("%s", fieldInfo.name.c_str());
    ImGui::SameLine();
    ImGui::Selectable(ptr->Get()->GetName().c_str());
    
    if (ImGui::BeginDragDropTarget())
    {
        // ReSharper disable once CppTooWideScope
        const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("ContentBrowserFile");
                
        if (payload)
        {
            XnorCore::Pointer<XnorCore::Resource> dragged = *static_cast<XnorCore::Pointer<XnorCore::Resource>*>(payload->Data);
            
            XnorCore::Resource* raw = static_cast<XnorCore::Resource*>(dragged);
            if (typeid(*raw).hash_code() == fieldInfo.typeHash)
            {
                *XnorCore::Utils::GetAddress<decltype(dragged)>(obj, fieldInfo.offset, 0) = dragged;
            }
        }
                
        ImGui::EndDragDropTarget();
    }
}

void Inspector::DisplayNativeType(void* const obj, const XnorCore::FieldInfo& fieldInfo, const char* const name, const size_t element)
{
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
    else if (fieldInfo.typeHash == typeid(bool_t).hash_code())
    {
        ImGui::Checkbox(name, XnorCore::Utils::GetAddress<bool_t>(obj, fieldInfo.offset, element));
    }
}

void Inspector::DisplayMathType(void* const obj, const XnorCore::FieldInfo& fieldInfo, const char* const name, const size_t element)
{
    if (fieldInfo.typeHash == typeid(Vector2i).hash_code())
    {
        ImGui::DragInt2(name, XnorCore::Utils::GetAddress<Vector2i>(obj, fieldInfo.offset, element)->Raw(), 0.1f);
    }
    else if (fieldInfo.typeHash == typeid(Vector2).hash_code())
    {
        ImGui::DragFloat2(name, XnorCore::Utils::GetAddress<Vector2>(obj, fieldInfo.offset, element)->Raw(), 0.1f);
    }
    else if (fieldInfo.typeHash == typeid(Vector3).hash_code())
    {
        ImGui::DragFloat3(name, XnorCore::Utils::GetAddress<Vector3>(obj, fieldInfo.offset, element)->Raw(), 0.1f);
    }
    else if (fieldInfo.typeHash == typeid(Vector4).hash_code())
    {
        ImGui::DragFloat4(name, XnorCore::Utils::GetAddress<Vector4>(obj, fieldInfo.offset, element)->Raw(), 0.1f);
    }
    else if (fieldInfo.typeHash == typeid(Quaternion).hash_code())
    {
        Quaternion* const q = XnorCore::Utils::GetAddress<Quaternion>(obj, fieldInfo.offset, element);

        Vector3 euler = XnorCore::Utils::GetQuaternionEulerAngles(*q);
        ImGui::SliderAngle3(name, euler.Raw());
        
        *q = Quaternion::FromEuler(euler);
    }
}

void Inspector::DisplayNestedType(void* const obj, const XnorCore::FieldInfo& fieldInfo, const char* name, const size_t element)
{
    if (!ImGui::CollapsingHeader(name))
        return;

    size_t hash = fieldInfo.typeHash;
    const bool isPoly = fieldInfo.isPolyPointer; 

    PolyPtr<void*>* polyPtr = nullptr;
    if (isPoly)
    {
        polyPtr = XnorCore::Utils::GetAddress<PolyPtr<void*>>(obj, fieldInfo.offset, element); 
        hash = polyPtr->GetHash();
    }

    if (!XnorCore::TypeInfo::Contains(hash))
    {
        ImGui::Text("Type isn't supported");
        return;
    }
    
    const XnorCore::TypeInfo& subInfo = XnorCore::TypeInfo::Get(hash);

    const char* const className = subInfo.GetName().c_str(); 
    XnorCore::Utils::AlignImGuiCursor(ImGui::CalcTextSize(className).x);
    ImGui::Text("%s", className);

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
