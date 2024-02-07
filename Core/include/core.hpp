#pragma once

#ifdef XNOR_EXPORTS
#define XNOR_ENGINE __declspec(dllexport)
#else
#define XNOR_ENGINE __declspec(dllimport)
#endif
