#pragma once

#include <array>

#include "core.hpp"

BEGIN_XNOR_CORE

template<typename T, T... Values>
class ArrayTest
{
public:
    // ReSharper disable once CppNonExplicitConversionOperator
    constexpr operator std::array<T, sizeof...(Values)>() const;
    
private:
    constexpr std::array<T, sizeof...(Values)> m_Array{T(Values)...};
};

template<typename T, T... Values>
constexpr ArrayTest<T, Values...>::operator std::array<T, sizeof...(Values)>() const
{
    return m_Array;
}

END_XNOR_CORE
