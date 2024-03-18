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
constexpr const AttributeT& Reflection::GetAttribute(const DescriptorT descriptor)
{
    return refl::descriptor::get_attribute<AttributeT>(descriptor);
}

template <typename AttributeT, typename DescriptorT>
constexpr const AttributeT* Reflection::TryGetAttribute(DescriptorT descriptor)
{
    if constexpr (HasAttribute<AttributeT, DescriptorT>(descriptor))
        return &GetAttribute<AttributeT, DescriptorT>(descriptor);
    else
        return nullptr;
}

END_XNOR_CORE
