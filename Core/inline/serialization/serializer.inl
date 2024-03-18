#pragma once

#include "serialization/serializer.hpp"

#include <format>

#include "magic_enum/magic_enum.hpp"
#include "magic_enum/magic_enum_flags.hpp"
#include "reflection/reflection.hpp"
#include "utils/guid.hpp"
#include "utils/logger.hpp"
#include "utils/meta_programming.hpp"

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
    else
        BeginXmlElement(typeName.c_str(), "");

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
    else
        EndXmlElement();
}

template <typename ReflectT, bool_t IsRoot>
void Serializer::Deserialize(ReflectT* const obj)
{
    // TODO static variables
    
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    constexpr const char_t* const typeName = desc.name.c_str();
    const std::string humanizedTypeName = Utils::RemoveNamespaces(typeName);

    ReadElement(humanizedTypeName);
    if constexpr (IsRoot)
    {
        
    }

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

            ReadElement(name);
            
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

            FinishReadElement();
        }
    });

    FinishReadElement();
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::SerializeSimpleType(const Metadata<ReflectT, MemberT, DescriptorT>& metadata)
{
    if constexpr (Meta::IsNativeType<MemberT> || Meta::IsMathType<MemberT> || Meta::IsSame<MemberT, std::string>)
    {
        AddSimpleValue<MemberT>(metadata.name, *metadata.obj);
    }
    else if constexpr (Meta::IsColorType<MemberT>)
    {
        // DisplayColor<MemberT>(ptr, name);
    }
    else if constexpr (Meta::IsPointer<MemberT>)
    {
        using PtrT = Meta::RemovePointerSpecifier<MemberT>;
        
        if constexpr (Meta::IsAbstract<PtrT>)
        {
            const size_t hash = Utils::GetTypeHash<Component>(*metadata.obj);
            SerializeObjectUsingFactory(*metadata.obj, hash);
        }
        else if constexpr (Reflection::HasAttribute<Reflection::ExpandPointer, DescriptorT>())
        {
            Serialize<PtrT, false>(*metadata.obj);
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
        Serialize<MemberT, false>(metadata.obj);
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
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::DeserializeArrayType(const Metadata<ReflectT, MemberT, DescriptorT>&)
{
}

template <typename ReflectT, typename MemberT, typename DescriptorT>
void Serializer::DeserializeListType(const Metadata<ReflectT, MemberT, DescriptorT>&)
{
    
}

END_XNOR_CORE
