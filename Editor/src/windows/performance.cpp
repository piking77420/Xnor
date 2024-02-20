#include "windows/performance.hpp"

#include <format>

#undef APIENTRY
#include <windows.h>
#undef min
#undef max
#undef APIENTRY
#include <psapi.h>

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "Maths/calc.hpp"

using namespace XnorEditor;

Performance::Performance(Editor* const editor, const size_t sampleSize)
    : UiWindow(editor)
{
    m_FrameRateArray.resize(sampleSize);
    m_MemoryArray.resize(sampleSize);
}

void Performance::Update(const double_t currentTime)
{
    if (Calc::OnInterval(static_cast<float_t>(currentTime), static_cast<float_t>(m_TotalTime), UpdateInterval))
    {
        const double_t deltaTime = currentTime - m_TotalTime;
        
        m_LastFps = static_cast<float_t>(1.0 / deltaTime);
        m_FrameRateArray[m_ArrayIndex] = m_LastFps;
        m_HighestArrayFps = std::max(*std::ranges::max_element(m_FrameRateArray), MinHighestArrayFps);

        PROCESS_MEMORY_COUNTERS memory;
        GetProcessMemoryInfo(GetCurrentProcess(), &memory, sizeof(memory));
        
        m_LastMemory = static_cast<float_t>(memory.WorkingSetSize) / 0x100000;
        m_MemoryArray[m_ArrayIndex] = m_LastMemory;
        m_HighestArrayMemory = std::max(*std::ranges::max_element(m_MemoryArray), MinHighestArrayMemory);

        m_ArrayIndex = static_cast<int32_t>((m_ArrayIndex + 1) % m_FrameRateArray.size());
        if (m_TotalSamples < MaxSamples)
            m_TotalSamples++;
    }
    
    m_TotalTime = currentTime;
}

void Performance::Display()
{
    // TODO time
    Update(0.0);
    
    ImGui::Begin("Performance Summary");

    const ImVec2 available = ImGui::GetContentRegionAvail();
    std::string format = std::format("FPS: {:.0f}", m_LastFps);
    FetchInfo();

    ImGui::PlotLines("##fps", m_FrameRateArray.data(), static_cast<int32_t>(std::min(m_TotalSamples, m_FrameRateArray.size())), m_ArrayIndex,
        format.c_str(), 0.f, m_HighestArrayFps, ImVec2(available.x, 50.f));

    format = std::format("Memory: {:.2f}MB", m_LastMemory);
    ImGui::PlotLines("##memory", m_MemoryArray.data(), static_cast<int32_t>(std::min(m_TotalSamples, m_MemoryArray.size())), m_ArrayIndex,
        format.c_str(), 0.f, m_HighestArrayMemory, ImVec2(available.x, 50.f));

    ImGui::End();
}
