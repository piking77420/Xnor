#pragma once

#include <crtdbg.h>

#include "core.hpp"

BEGIN_XNOR_CORE

class MemoryLeakDetector
{
public:
    XNOR_ENGINE MemoryLeakDetector();

    XNOR_ENGINE ~MemoryLeakDetector();

    MemoryLeakDetector(const MemoryLeakDetector& other) = delete;
    
    MemoryLeakDetector(MemoryLeakDetector&& other) = delete;
    
    MemoryLeakDetector& operator=(const MemoryLeakDetector& other) = delete;
    
    MemoryLeakDetector& operator=(MemoryLeakDetector&& other) = delete;

private:
    static void ReportFailure(const size_t unfreedBytes);

    _CrtMemState m_MemState;
};

END_XNOR_CORE
