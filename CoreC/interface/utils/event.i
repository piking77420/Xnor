%module CoreC

%include "utils/event.hpp"

%template(CollisionExitEvent) XnorCore::Event<XnorCore::Collider*, XnorCore::Collider*>;
%template(CollisionEvent) XnorCore::Event<XnorCore::Collider*, XnorCore::Collider*, const XnorCore::CollisionData&>;
