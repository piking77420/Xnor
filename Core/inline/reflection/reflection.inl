#pragma once

#include "utils/logger.hpp"
#include "utils/meta_programming.hpp"

BEGIN_XNOR_CORE

template <typename ReflectT>
constexpr TypeDescriptor<ReflectT> Reflection::GetTypeInfo()
{
    return refl::reflect<ReflectT>();
}

template <typename AttributeT, typename DescriptorT>
constexpr bool_t Reflection::HasAttribute()
{
    return refl::descriptor::has_attribute<AttributeT, DescriptorT>(DescriptorT{});
}

template <typename AttributeT, typename DescriptorT>
constexpr const AttributeT& Reflection::GetAttribute()
{
    return refl::descriptor::get_attribute<AttributeT, DescriptorT>(DescriptorT{});
}

template <typename AttributeT, typename DescriptorT>
constexpr const AttributeT* Reflection::TryGetAttribute()
{
    if constexpr (HasAttribute<AttributeT, DescriptorT>())
        return &GetAttribute<AttributeT, DescriptorT>();
    else
        return nullptr;
}

template <typename T>
void Reflection::Clone(const T* const, T* const)
{
    /*if constexpr (!Reflection::IsReflected<T>)
    {
        Logger::LogError("Trying to clone a non reflected type {}", typeid(T).name());
        return;
    }

    refl::util::for_each(decltype(Reflection::GetTypeInfo<T>())::members, [&]<typename DescriptorT>(const DescriptorT)
    {
        using MemberT = typename DescriptorT::value_type;

        if constexpr (Meta::IsXnorList<MemberT>)
        {
            using ListT = typename MemberT::Type;

            const MemberT& srcList = DescriptorT::get(src);
            MemberT& dstList = DescriptorT::get(dst);

            if constexpr (Meta::IsPointer<ListT> && Meta::IsAbstract<ListT>)
            {
                for (size_t i = 0; i < srcList.GetSize(); i++)
                {
                    const size_t hash = typeid(*srcList[i]).hash_code();
                    dstList.Add(static_cast<ListT*>(Reflection::CreateUsingFactory(hash)));
                    Reflection::CloneUsingFactory(srcList[i], dstList[i], hash);
                }
            }
            else
            {
                for (size_t i = 0; i < srcList.GetSize(); i++)
                {
                    dstList.Add(srcList[i]);
                }
            }
        }
        else if constexpr (Meta::IsCopyAssignable<MemberT>)
        {
            DescriptorT::get(dst) = DescriptorT::get(src);
        }
    });*/
}

END_XNOR_CORE
