#pragma once

#include <vector>

#include "definitions.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class Performance : public UiWindow
{
public:
    Performance();
    
    void Update(double currentTime);

    void Display() override;

private:
    // TEMPORARY
    double_t totalTime = 0;
    static constexpr inline float_t UpdateInterval = 0.5f;
    
    size_t totalSamples = 0;
    static constexpr inline uint32_t MaxSamples = 50;
    int32_t arrayIndex = 0;

    float_t lastFps = 0.f;
    std::vector<float_t> frameRateArray;
    float_t highestArrayFps = 0.f;
    static constexpr inline float_t MinHighestArrayFps = 60.f;
        
    float_t lastMemory = 0.f;
    std::vector<float_t> memoryArray;
    float_t highestArrayMemory = 0.f;
    static constexpr inline float_t MinHighestArrayMemory = 50.f;
};

END_XNOR_EDITOR
