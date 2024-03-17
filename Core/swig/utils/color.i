%module Core

%csmethodmodifiers XnorCore::ColorRgb::operator ColorRgba "private";
%csmethodmodifiers XnorCore::ColorRgb::operator ColorHsva "private";
%csmethodmodifiers XnorCore::ColorRgb::operator Colorf "private";

%rename(ToColorRgba) XnorCore::ColorRgb::operator ColorRgba;
%rename(ToColorHsva) XnorCore::ColorRgb::operator ColorHsva;
%rename(ToColorf) XnorCore::ColorRgb::operator Colorf;

%typemap(cscode) XnorCore::ColorRgb
%{
    public static ColorRgb operator+(ColorRgb a, ColorRgb b) { return Core.OperatorAdd(a, b); }

    public static ColorRgb operator*(ColorRgb a, ColorRgb b) { return Core.OperatorMul(a, b); }

    public static explicit operator ColorRgba(ColorRgb color) { return color.ToColorRgba(); }

    public static explicit operator ColorHsva(ColorRgb color) { return color.ToColorHsva(); }

    public static explicit operator Colorf(ColorRgb color) { return color.ToColorf(); }
%}

%csmethodmodifiers XnorCore::ColorRgba::operator ColorRgb "private";
%csmethodmodifiers XnorCore::ColorRgba::operator ColorHsva "private";
%csmethodmodifiers XnorCore::ColorRgba::operator Colorf "private";

%rename(ToColorRgb) XnorCore::ColorRgba::operator ColorRgb;
%rename(ToColorHsva) XnorCore::ColorRgba::operator ColorHsva;
%rename(ToColorf) XnorCore::ColorRgba::operator Colorf;

%typemap(cscode) XnorCore::ColorRgba
%{
    public static ColorRgba operator+(ColorRgba a, ColorRgba b) { return Core.OperatorAdd(a, b); }

    public static ColorRgba operator*(ColorRgba a, ColorRgba b) { return Core.OperatorMul(a, b); }

    public static ColorRgba operator*(ColorRgba c, float alphaFactor) { return Core.OperatorMul(c, alphaFactor); }

    public static explicit operator ColorRgb(ColorRgba color) { return color.ToColorRgb(); }

    public static explicit operator ColorHsva(ColorRgba color) { return color.ToColorHsva(); }

    public static explicit operator Colorf(ColorRgba color) { return color.ToColorf(); }
%}

%csmethodmodifiers XnorCore::Colorf::operator ColorRgb "private";
%csmethodmodifiers XnorCore::Colorf::operator ColorRgba "private";
%csmethodmodifiers XnorCore::Colorf::operator ColorHsva "private";
%csmethodmodifiers XnorCore::Colorf::operator Vector3 "private";
%csmethodmodifiers XnorCore::Colorf::operator Vector4 "private";

%rename(ToColorRgb) XnorCore::Colorf::operator ColorRgb;
%rename(ToColorRgba) XnorCore::Colorf::operator ColorRgba;
%rename(ToColorHsva) XnorCore::Colorf::operator ColorHsva;
%rename(ToVector3) XnorCore::Colorf::operator Vector3;
%rename(ToVector4) XnorCore::Colorf::operator Vector4;

%typemap(cscode) XnorCore::Colorf
%{
    public static Colorf operator+(Colorf a, Colorf b) { return Core.OperatorAdd(a, b); }

    public static Colorf operator*(Colorf a, Colorf b) { return Core.OperatorMul(a, b); }

    public static Colorf operator*(Colorf c, float alphaFactor) { return Core.OperatorMul(c, alphaFactor); }

    public static explicit operator ColorRgb(Colorf color) { return color.ToColorRgb(); }

    public static explicit operator ColorRgba(Colorf color) { return color.ToColorRgba(); }

    public static explicit operator ColorHsva(Colorf color) { return color.ToColorHsva(); }

    public static explicit operator Vector3(Colorf color) { return color.ToVector3(); }

    public static explicit operator Vector4(Colorf color) { return color.ToVector4(); }
%}

%csmethodmodifiers XnorCore::ColorHsva::operator ColorRgb "private";
%csmethodmodifiers XnorCore::ColorHsva::operator ColorRgba "private";
%csmethodmodifiers XnorCore::ColorHsva::operator Colorf "private";

%rename(ToColorRgb) XnorCore::ColorHsva::operator ColorRgb;
%rename(ToColorRgba) XnorCore::ColorHsva::operator ColorRgba;
%rename(ToColorf) XnorCore::ColorHsva::operator Colorf;

%typemap(cscode) XnorCore::ColorHsva
%{
    public static ColorHsva operator*(ColorHsva c, float alphaFactor) { return Core.OperatorMul(c, alphaFactor); }

    public static explicit operator ColorRgb(ColorHsva color) { return color.ToColorRgb(); }

    public static explicit operator ColorRgba(ColorHsva color) { return color.ToColorRgba(); }

    public static explicit operator Colorf(ColorHsva color) { return color.ToColorf(); }
%}

%include "utils/color.hpp"
