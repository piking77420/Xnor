%module CoreC

%typemap(csclassmodifiers) XnorCore::Component "public abstract class";

%csmethodmodifiers XnorCore::Component::Begin "protected virtual";
%csmethodmodifiers XnorCore::Component::Update "protected virtual";

%include "scene/component.hpp"
