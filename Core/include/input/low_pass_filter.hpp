#pragma once

#include <array>

#include "core.hpp"
#include "utils/meta_programming.hpp"


BEGIN_XNOR_CORE
template <typename T,size_t size>
class LowPassFilter
{
public:
    using Type = T;

    DEFAULT_COPY_MOVE_OPERATIONS(LowPassFilter)
    
    LowPassFilter() = default;

    ~LowPassFilter() = default;

    template<typename U>
    U GetAvarage();
    
    void AddSample(T sample);

private:
    std::array<Type,size> m_Data;
    T m_Sum;
    uint32_t m_CurrentFrameIndex;
};

template <typename T, size_t size>
template <typename U>
U LowPassFilter<T, size>::GetAvarage()
{
    static_assert(size != 0, "Size can't be equal to zero");
    
    return m_Sum / size;
}

template <typename T, size_t size>
void LowPassFilter<T, size>::AddSample(T sample)
{
    m_Sum -= m_Data[m_CurrentFrameIndex];
    m_Data.at(m_CurrentFrameIndex) = sample;
    m_Sum += sample;
    m_CurrentFrameIndex += 1;

    if (m_CurrentFrameIndex >= size)
        m_CurrentFrameIndex = 0;
    
}

END_XNOR_CORE
