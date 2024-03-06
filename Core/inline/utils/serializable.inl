#pragma once

BEGIN_XNOR_CORE

template <typename ReflectT>
constexpr TypeDescriptor<ReflectT> Reflection::GetTypeInfo()
{
    return refl::reflect<ReflectT>();
}

template <typename AttributeT, typename DescriptorT>
constexpr bool_t Reflection::HasAttribute(const DescriptorT descriptor)
{
    return refl::descriptor::has_attribute<AttributeT>(descriptor);
}

template <typename AttributeT, typename DescriptorT>
constexpr AttributeT Reflection::GetAttribute(const DescriptorT descriptor)
{
    return refl::descriptor::get_attribute<AttributeT>(descriptor);
}

END_XNOR_CORE
