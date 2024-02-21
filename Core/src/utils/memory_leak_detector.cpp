#include "utils/memory_leak_detector.hpp"

#include <iostream>

#ifdef _DEBUG
using namespace XnorCore;

MemoryLeakDetector::MemoryLeakDetector()
{
    _CrtMemCheckpoint(&m_MemState);
}

MemoryLeakDetector::~MemoryLeakDetector()
{
    _CrtMemState stateNow, stateDiff;
    _CrtMemCheckpoint(&stateNow);
    if (_CrtMemDifference(&stateDiff, &m_MemState, &stateNow))
        ReportFailure(stateDiff.lSizes[1]);
}

void MemoryLeakDetector::ReportFailure(const size_t unfreedBytes)
{
    std::cerr << "Memory leak of " << unfreedBytes << " (0x" << std::hex << unfreedBytes << std::dec << ") (signed: " << static_cast<int64_t>(unfreedBytes) << ") byte(s) detected" << std::endl;
}
#endif
