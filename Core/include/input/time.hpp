#pragma once


#include <cmath>

#include "core.hpp"

BEGIN_XNOR_CORE
class Time
{
public:
    
    Time() = delete;
    
    ~Time() = delete;
    
    XNOR_ENGINE static double GetTimeDouble();

    XNOR_ENGINE static float_t GetTime();

    XNOR_ENGINE static void ComputeDeltaTime();

    XNOR_ENGINE static float_t GetDeltaTime();
    
private:
    XNOR_ENGINE static inline float_t m_CurrentFrame = 0.f;
    XNOR_ENGINE static inline float_t m_LastFrame = 0.f;
    XNOR_ENGINE static inline float_t m_DeltaTime = 0.f;

};


END_XNOR_CORE
