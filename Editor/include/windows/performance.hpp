#pragma once

#include <vector>

#include "definitions.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class Performance : public UiWindow
{
public:
    explicit Performance(Editor* editor, size_t sampleSize);
    
    void Update(double_t currentTime);

    void Display() override;

private:
    // TEMPORARY
    double_t m_TotalTime = 0;
    
    static constexpr inline float_t UpdateInterval = 0.5f;
    
    size_t m_TotalSamples = 0;
    static constexpr inline uint32_t MaxSamples = 50;
    int32_t m_ArrayIndex = 0;

    float_t m_LastFps = 0.f;
    std::vector<float_t> m_FrameRateArray;
    float_t m_HighestArrayFps = 0.f;
    static constexpr inline float_t MinHighestArrayFps = 60.f;
        
    float_t m_LastMemory = 0.f;
    std::vector<float_t> m_MemoryArray;
    float_t m_HighestArrayMemory = 0.f;
    static constexpr inline float_t MinHighestArrayMemory = 50.f;
};

END_XNOR_EDITOR
