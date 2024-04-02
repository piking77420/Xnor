%module CoreC

%include "typemaps/cs_struct.i"

%ignore XnorCore::ColorRgb;
%ignore XnorCore::ColorRgba;
%ignore XnorCore::Colorf;
%ignore XnorCore::ColorHsva;

%cs_struct2(XnorCore::ColorRgb, Data4, ColorRgb);
%cs_struct2(XnorCore::ColorRgba, Data4, ColorRgba);
%cs_struct2(XnorCore::Colorf, Data16, Colorf);
%cs_struct2(XnorCore::ColorHsva, Data4, ColorHsva);

namespace XnorCore
{
    struct ColorRgb {};
    struct ColorRgba {};
    struct Colorf {};
    struct ColorHsva {};
}
