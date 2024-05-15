%module CoreC

%typemap(csclassmodifiers) XnorCore::Component "public abstract class";

%include "scene/component.hpp"
