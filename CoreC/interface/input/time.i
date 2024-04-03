%module CoreC

%include "typemaps/cs_static_class.i"

%typemap(csclassmodifiers) XnorCore::Time "public static class";

%cs_static_class(XnorCore::Time);

%include "input/time.hpp"

%template(GetTotalTime) XnorCore::Time::GetTotalTime<float_t>;
%template(GetLastTotalTime) XnorCore::Time::GetLastTotalTime<float_t>;
%template(GetDeltaTime) XnorCore::Time::GetDeltaTime<float_t>;
%template(GetTotalFrameCount) XnorCore::Time::GetTotalFrameCount<uint32_t>;
