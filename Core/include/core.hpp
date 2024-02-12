#pragma once

#include <cstdint>

#ifdef XNOR_EXPORT
#define XNOR_ENGINE __declspec(dllexport)
#else
#define XNOR_ENGINE __declspec(dllimport)
#endif

#define BEGIN_XNOR_CORE namespace XnorCore {

#define END_XNOR_CORE }
