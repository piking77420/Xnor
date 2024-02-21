#pragma once

#include <crtdbg.h>

#include "core.hpp"

BEGIN_XNOR_CORE

class MemoryLeakDetector
{
    // The functions we use in this class come from the windows API and are
    // disabled in optimized builds
#ifdef _DEBUG
public:
    XNOR_ENGINE MemoryLeakDetector();

    XNOR_ENGINE ~MemoryLeakDetector();

    MemoryLeakDetector(const MemoryLeakDetector& other) = delete;
    
    MemoryLeakDetector(MemoryLeakDetector&& other) = delete;
    
    MemoryLeakDetector& operator=(const MemoryLeakDetector& other) = delete;
    
    MemoryLeakDetector& operator=(MemoryLeakDetector&& other) = delete;

private:
    static void ReportFailure(size_t unfreedBytes);

    _CrtMemState m_MemState;
#endif
};

END_XNOR_CORE
