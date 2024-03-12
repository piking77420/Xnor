#pragma once

#include "serialization/serializer.hpp"

#include <format>

#include "rendering/light/directional_light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "scene/component/test_component.hpp"
#include "utils/guid.hpp"
#include "utils/logger.hpp"
#include "utils/meta_programming.hpp"
#include "utils/serializable.hpp"

BEGIN_XNOR_CORE

template <typename T>
void Serializer::FetchAttribute(const std::string& attributeName, const T& value)
{
    if constexpr (Meta::IsAny<T, std::string, const char_t*>)
    {
        FetchAttributeInternal(attributeName, value);
    }
    else
    {
        const std::string valueString = std::format("{}", value);
        FetchAttributeInternal(attributeName, valueString);
    }
}

template <typename ReflectT>
void Serializer::Serialize(const ReflectT* const obj, const bool_t isRoot)
{
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    if (isRoot)
        BeginRootElement(desc.name.c_str(), "");
    else
        BeginXmlElement(desc.name.c_str(), "");

    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        constexpr bool_t isFunction = refl::descriptor::is_function(member);
        
        constexpr bool_t dontSerialize = Reflection::HasAttribute<Reflection::NotSerializable>(member);
        constexpr size_t flags = GetFlags<T>(member);
            
        if constexpr (!isFunction)
        {
            if constexpr (!dontSerialize)
            {
                using MemberT = typename T::value_type;
                const constexpr char_t* const name = member.name.c_str();

                if constexpr (Meta::IsArray<MemberT>)
                {
                    SerializeArrayType<MemberT>(&member.get(obj), name, flags);
                }
                else if constexpr (Meta::IsXnorList<MemberT>)
                {
                    SerializeListType<MemberT>(&member.get(obj), name, flags);
                }
                else
                {
                    SerializeSimpleType<MemberT>(&member.get(obj), name, flags);
                }
            }
        }
        else
        {
            Logger::LogInfo("{} ; {}", desc.name.c_str(), member.name.c_str());
        }
    });

    if (isRoot)
        EndRootElement();
    else
        EndXmlElement();
}

template <typename ReflectT>
void Serializer::Deserialize(ReflectT* const)
{
    // TODO Deserialize
}

template <typename MemberT>
void Serializer::SerializeSimpleType(const MemberT* ptr, const char_t* name, const size_t flags)
{
    if constexpr (Meta::IsNativeType<MemberT> || Meta::IsMathType<MemberT> || Meta::IsSame<MemberT, std::string>)
    {
        FetchAttribute<MemberT>(name, *ptr);
    }
    else if constexpr (Meta::IsColorType<MemberT>)
    {
        // DisplayColor<MemberT>(ptr, name);
    }
    else if constexpr (Meta::IsPointer<MemberT>)
    {
        // TODO fix serialization
        /*
        if (flags & EXPAND_POINTER)
        {
            Serialize<Meta::RemovePointerSpecifier<MemberT>>(*ptr, false);
        }
        else
        {
            if (*ptr == nullptr)
                FetchAttribute(name, Guid());
            else
                FetchAttribute(name, static_cast<std::string>((*ptr)->GetGuid()));
        }
        */
    }
    else if constexpr (Meta::IsPolyPtr<MemberT>)
    {
        const size_t hash = ptr->GetHash();
        
#define POLY_PTR_IF_SER(type)                       \
if (hash == XnorCore::Utils::GetTypeHash<type>())   \
{                                                   \
Serialize<type>(ptr->Cast<type>(), false);          \
}                                                   \
        // TODO find a less ugly solution to that

        POLY_PTR_IF_SER(MeshRenderer);
        POLY_PTR_IF_SER(DirectionalLight);
        POLY_PTR_IF_SER(TestComponent);
        POLY_PTR_IF_SER(PointLight);
        POLY_PTR_IF_SER(SpotLight);
    }
    else if constexpr (Meta::IsXnorPointer<MemberT>)
    {
        if (flags & EXPAND_POINTER)
        {
            // Serialize<Meta::RemovePointerSpecifier<typename MemberT::Type>>(*ptr, false);
        }
        else
        {
            if (*ptr == nullptr)
                FetchAttribute(name, Guid());
            else
                FetchAttribute(name, static_cast<std::string>((*ptr)->GetGuid()));
        }
    }
    else if constexpr (Meta::IsSame<MemberT, Guid>)
    {
        FetchAttribute<std::string>(name, static_cast<std::string>(*ptr));
    }
    else if constexpr (Meta::IsEnum<MemberT>)
    {
        // TODO enum serialization
    }
    else
    {
        Serialize<MemberT>(ptr, false);
    }
}

template <typename MemberT>
void Serializer::SerializeArrayType(const MemberT*, const char_t*, const size_t)
{
    // TODO SerializeArrayType
}

template <typename MemberT>
void Serializer::SerializeListType(const MemberT* ptr, const char_t* name, const size_t flags)
{
    using ListT = typename MemberT::Type;
    
    BeginXmlElement(std::string(name), "");

    for (size_t i = 0; i < ptr->GetSize(); i++)
    {
        SerializeSimpleType<ListT>(&(*ptr)[i], std::to_string(i).c_str(), flags);
    }
    
    EndXmlElement();
}

template <typename T>
constexpr size_t Serializer::GetFlags(const T member)
{
    size_t flags = NONE; 

    if constexpr (Reflection::HasAttribute<Reflection::ExpandPointer>(member))
        flags |= EXPAND_POINTER;

    return flags;
}

END_XNOR_CORE
