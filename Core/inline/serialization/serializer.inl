#pragma once

#include "serialization/serializer.hpp"

#include <format>
#include <stdlib.h>

#include "magic_enum/magic_enum.hpp"
#include "magic_enum/magic_enum_flags.hpp"
#include "reflection/reflection.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component.hpp"
#include "utils/guid.hpp"
#include "utils/logger.hpp"
#include "utils/meta_programming.hpp"
#include "world/world.hpp"

BEGIN_XNOR_CORE
    template <typename T>
void Serializer::AddSimpleValue(const std::string& attributeName, const T& value)
{
    if constexpr (Meta::IsAny<T, std::string, const char_t*>)
    {
        BeginXmlElement(attributeName, value);
    }
    else
    {
        const std::string valueString = std::format("{}", value);
        BeginXmlElement(attributeName, valueString);
    }

    EndXmlElement();
}

template <typename ReflectT, bool_t IsRoot>
void Serializer::Serialize(const ReflectT* const obj)
{
    // TODO static variables

    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    const std::string typeName = Utils::RemoveNamespaces(desc.name.c_str());

    if constexpr (IsRoot)
        BeginRootElement(typeName.c_str(), "");
    // else
        // BeginXmlElement(typeName.c_str(), "");

    refl::util::for_each(desc.members, [&]<typename T>(const T)
    {
        constexpr bool_t dontSerialize = Reflection::HasAttribute<Reflection::NotSerializable, T>();
        constexpr bool_t isStatic = T::is_static;
            
        if constexpr (!dontSerialize && !isStatic)
        {
            using MemberT = Meta::RemoveConstSpecifier<typename T::value_type>;
            constexpr const char_t* const name = T::name.c_str();

            Metadata<ReflectT, MemberT, T> metadata = {
                .topLevelObj = const_cast<ReflectT*>(obj),
                .name = name,
                .obj = const_cast<MemberT*>(&T::get(obj))
            };

            if constexpr (Meta::IsArray<MemberT>)
            {
                SerializeArrayType<ReflectT, MemberT, T>(metadata);
            }
            else if constexpr (Meta::IsXnorList<MemberT>)
            {
                SerializeListType<ReflectT, MemberT, T>(metadata);
            }
            else
            {
                SerializeSimpleType<ReflectT, MemberT, T>(metadata);
            }
        }
    });

    if constexpr (IsRoot)
        EndRootElement();
    // else
       // EndXmlElement();
}

template <typename ReflectT, bool_t IsRoot>
void Serializer::Deserialize(ReflectT* const obj)
{
    // TODO static variables
    
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    constexpr const char_t* const typeName = desc.name.c_str();
    const std::string humanizedTypeName = Utils::RemoveNamespaces(typeName);

    if constexpr (IsRoot)
        ReadElement(humanizedTypeName);

    refl::util::for_each(desc.members, [&]<typename T>(const T)
    {
        constexpr bool_t dontSerialize = Reflection::HasAttribute<Reflection::NotSerializable, T>();
        constexpr bool_t isStatic = T::is_static;

        if constexpr (!dontSerialize && !isStatic)
        {
            using MemberT = Meta::RemoveConstSpecifier<typename T::value_type>;
            constexpr const char_t* const name = T::name.c_str();

            Metadata<ReflectT, MemberT, T> metadata = {
                .topLevelObj = const_cast<ReflectT*>(obj),
                .name = name,
                .obj = const_cast<MemberT*>(&T::get(obj))
            };

            if constexpr (Meta::IsArray<MemberT>)
            {
                DeserializeArrayType<ReflectT, MemberT, T>(metadata);
            }
            else if constexpr (Meta::IsXnorList<MemberT>)
            {
                DeserializeListType<ReflectT, MemberT, T>(metadata);
            }
            else
            {
                DeserializeSimpleType<ReflectT, MemberT, T>(metadata);
            }
        }
    });

    if constexpr (IsRoot)
    {
        FinishReadElement();

        for (auto&& it : m_GuidEntityMap)
        {
            *it.second = World::scene->GetEntityById(it.first);
        }
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::SerializeSimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Meta::IsNativeType<MemberT> || Meta::IsMathType<MemberT> || Meta::IsColorType<MemberT> || Meta::IsSame<MemberT, std::string>)
    {
        AddSimpleValue<MemberT>(metadata.name, *metadata.obj);
    }
    else if constexpr (Meta::IsPointer<MemberT>)
    {
        using PtrT = Meta::RemovePointerSpecifier<MemberT>;
        
        if constexpr (Meta::IsAbstract<PtrT>)
        {
            BeginXmlElement(metadata.name, "");
            const size_t hash = Utils::GetTypeHash<Component>(*metadata.obj);
            SerializeObjectUsingFactory(*metadata.obj, hash);
            EndXmlElement();
        }
        else if constexpr (Reflection::HasAttribute<Reflection::ExpandPointer, DescriptorT>())
        {
            BeginXmlElement(metadata.name, "");
            Serialize<PtrT, false>(*metadata.obj);
            EndXmlElement();
        }
        else
        {
            if (*metadata.obj == nullptr)
                AddSimpleValue(metadata.name, static_cast<std::string>(Guid()));
            else
                AddSimpleValue(metadata.name, static_cast<std::string>((*metadata.obj)->GetGuid()));
        }
    }
    else if constexpr (Meta::IsXnorPointer<MemberT>)
    {
        if (*metadata.obj == nullptr)
            AddSimpleValue(metadata.name, Guid());
        else
            AddSimpleValue(metadata.name, static_cast<std::string>((*metadata.obj)->GetGuid()));
    }
    else if constexpr (Meta::IsSame<MemberT, Guid>)
    {
        AddSimpleValue<std::string>(metadata.name, static_cast<std::string>(*metadata.obj));
    }
    else if constexpr (Meta::IsEnum<MemberT>)
    {
        SerializeEnum<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else
    {
        BeginXmlElement(metadata.name, "");
        Serialize<MemberT, false>(metadata.obj);
        EndXmlElement();
    }
}


template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::SerializeArrayType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using ArrayT = Meta::RemoveArraySpecifier<MemberT>;
    constexpr size_t size = sizeof(MemberT) / sizeof(ArrayT); 

    BeginXmlElement(metadata.name, "");

    Metadata<ReflectT, ArrayT, DescriptorT> metadataList = {
        .topLevelObj = metadata.topLevelObj
    };

    for (size_t i = 0; i < size; i++)
    {
        const std::string index = std::to_string(i);
        metadataList.name = index.c_str();
        metadataList.obj = &(*metadata.obj)[i];
        SerializeSimpleType<ReflectT, ArrayT, DescriptorT>(metadataList);
    }
    
    EndXmlElement();
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::SerializeListType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using ListT = typename MemberT::Type;
    
    BeginXmlElement(metadata.name, "");

    const size_t size = metadata.obj->GetSize();

    Metadata<ReflectT, ListT, DescriptorT> metadataList = {
        .topLevelObj = metadata.topLevelObj
    };

    for (size_t i = 0; i < size; i++)
    {
        const std::string index = std::to_string(i);
        metadataList.name = index.c_str();
        metadataList.obj = &(*metadata.obj)[i];
        SerializeSimpleType<ReflectT, ListT, DescriptorT>(metadataList);
    }
    
    EndXmlElement();
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::SerializeEnum(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    constexpr bool_t isEnumFlag = Reflection::HasAttribute<Reflection::EnumFlags, DescriptorT>();
    constexpr auto enumNames = magic_enum::enum_names<MemberT>();
    
    if constexpr (isEnumFlag)
    {
        AddSimpleValue<std::string>(metadata.name, magic_enum::enum_flags_name<MemberT>(*metadata.obj, ',').data());
    }
    else
    {
        AddSimpleValue<std::string>(metadata.name, magic_enum::enum_name<MemberT>(*metadata.obj).data());
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::DeserializeSimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Meta::IsSame<MemberT, bool_t>)
    {
        const char_t* const v = ReadElementValue(metadata.name);
        if (std::strcmp(v, "true") == 0)
            *metadata.obj = true;
        else
            *metadata.obj = false;
    }
    else if constexpr (Meta::IsIntegral<MemberT>)
    {
        *metadata.obj = std::atoi(ReadElementValue(metadata.name));
    }
    else if constexpr (Meta::IsFloatingPoint<MemberT>)
    {
        *metadata.obj = std::stof(ReadElementValue(metadata.name));
    }
    else if constexpr (Meta::IsSame<MemberT, Vector2i>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%d ; %d", &metadata.obj->x, &metadata.obj->y);
    }
    else if constexpr (Meta::IsSame<MemberT, Vector2>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%f ; %f", &metadata.obj->x, &metadata.obj->y);
    }
    else if constexpr (Meta::IsSame<MemberT, Vector3>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%f ; %f ; %f", &metadata.obj->x, &metadata.obj->y, &metadata.obj->z);
    }
    else if constexpr (Meta::IsSame<MemberT, Vector4>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%f ; %f ; %f ; %f", &metadata.obj->x, &metadata.obj->y, &metadata.obj->z, &metadata.obj->w);
    }
    else if constexpr (Meta::IsSame<MemberT, Quaternion>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%f ; %f ; %f ; %f", &metadata.obj->X(), &metadata.obj->Y(), &metadata.obj->Z(), &metadata.obj->W());
    }
    else if constexpr (Meta::IsSame<MemberT, ColorRgb>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%hhu ; %hhu ; %hhu", &metadata.obj->r, &metadata.obj->g, &metadata.obj->b);
    }
    else if constexpr (Meta::IsSame<MemberT, ColorRgba>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%hhu ; %hhu ; %hhu ; %hhu", &metadata.obj->r, &metadata.obj->g, &metadata.obj->b, &metadata.obj->a);
    }
    else if constexpr (Meta::IsSame<MemberT, Colorf>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%f ; %f ; %f ; %f", &metadata.obj->r, &metadata.obj->g, &metadata.obj->b, &metadata.obj->a);
    }
    else if constexpr (Meta::IsSame<MemberT, ColorHsva>)
    {
        sscanf_s(ReadElementValue(metadata.name), "%hhu ; %hhu ; %hhu ; %hhu", &metadata.obj->h, &metadata.obj->s, &metadata.obj->v, &metadata.obj->a);
    }
    else if constexpr (Meta::IsEnum<MemberT>)
    {
        // TODO enum
    }
    else if constexpr (Meta::IsSame<MemberT, std::string>)
    {
        *metadata.obj = std::string(ReadElementValue(metadata.name));
    }
    else if constexpr (Meta::IsPointer<MemberT>)
    {
        DeserializePointer<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsXnorPointer<MemberT>)
    {
        DeserializeXnorPointer<ReflectT, MemberT, DescriptorT>(metadata);
    }
    else if constexpr (Meta::IsSame<MemberT, Guid>)
    {
        const char_t* const value = ReadElementValue(metadata.name);

        *metadata.obj = Guid::FromString(value);
    }
    else
    {
        ReadElement(metadata.name);
        Deserialize<MemberT, false>(metadata.obj);
        FinishReadElement();
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::DeserializePointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using PtrT = Meta::RemovePointerSpecifier<MemberT>;
    
    if constexpr (Reflection::HasAttribute<Reflection::ExpandPointer, DescriptorT>())
    {
        ReadElement(metadata.name);
        Deserialize<PtrT, false>(*metadata.obj);
        FinishReadElement();
    }
    else if constexpr (Meta::IsAbstract<PtrT>)
    {
        ReadElement(metadata.name);

        const size_t hash = Utils::GetTypeHash<PtrT>(*metadata.obj);
        DeserializeObjectUsingFactory(*metadata.obj, hash);
        FinishReadElement();
    }
    else if constexpr (Meta::IsSame<PtrT, Entity>)
    {
        const Guid guid = Guid::FromString(ReadElementValue(metadata.name));
        if (guid == Guid())
            *metadata.obj = nullptr;
        else
            m_GuidEntityMap.emplace(guid, metadata.obj);
    }
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::DeserializeXnorPointer(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using PtrT = typename MemberT::Type;
    
    const char_t* const value = ReadElementValue(metadata.name);
    const Guid guid = Guid::FromString(value);

    if (guid == Guid())
    {
        *metadata.obj = nullptr;
        return;
    }

    Pointer<PtrT> res = ResourceManager::Get<PtrT>(guid);

    if (res == nullptr)
    {
        Logger::LogError("Resource with guid {} wasn't found (deserializing {})", static_cast<std::string>(guid), metadata.name);
    }
    else
    {
        Logger::LogInfo("Resource with guid {} was found (deserializing {}, {})", static_cast<std::string>(guid), metadata.name, res->GetName());
    }

    *metadata.obj = res;
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::DeserializeArrayType(const Metadata<ReflectT, MemberT, DescriptorT>&)
{
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::DeserializeListType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    using ListT = typename MemberT::Type;

    ReadElement(metadata.name);

    // metadata.obj->Clear();

    const XMLElement* const parent = m_ElementsStack.top();
    const XMLElement* child = parent->first_node("0");

    size_t i = 0;
    for (; child != nullptr; child = child->next_sibling())
    {
        using PtrT = Meta::RemovePointerSpecifier<ListT>;

        if constexpr (Meta::IsAbstract<PtrT>)
        {
            ListT const ptr = static_cast<ListT>(CreateObjectUsingFactory(std::string(child->first_attribute("typeName")->value())));
            metadata.obj->Add(ptr);
            if constexpr (Meta::IsSame<PtrT, Component>)
                 ptr->entity = metadata.topLevelObj;
        }
        else if constexpr (Meta::IsPointer<ListT>)
        {
            metadata.obj->Add(new PtrT());
        }
        else
        {
            metadata.obj->Add();
        }

        const std::string index = std::to_string(i);
        Metadata<ReflectT, ListT, DescriptorT> metadataArray = {
            .topLevelObj = metadata.topLevelObj,
            .name = index.c_str(),
            .obj = &(*metadata.obj)[i]
        };

        DeserializeSimpleType<ReflectT, ListT, DescriptorT>(metadataArray);

        i++;
    }

    FinishReadElement();
}

END_XNOR_CORE
