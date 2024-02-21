#pragma once

#include <vector>

#include "definitions.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class Performance : public UiWindow
{
public:
    explicit Performance(Editor* editor, size_t sampleSize);
    
    void Update();

    void Display() override;

private:
    static constexpr inline float_t UpdateInterval = 0.1f;
    static constexpr inline float_t GraphsHeight = 50.f;
    
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
