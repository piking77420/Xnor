#pragma once

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

END_XNOR_CORE
