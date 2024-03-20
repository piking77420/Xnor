%module CoreC

%include "input/time.hpp"

%template(GetTotalTime) XnorCore::Time::GetTotalTime<float_t>;
%template(GetLastTotalTime) XnorCore::Time::GetLastTotalTime<float_t>;
%template(GetDeltaTime) XnorCore::Time::GetDeltaTime<float_t>;
%template(GetTotalFrameCount) XnorCore::Time::GetTotalFrameCount<uint32_t>;
