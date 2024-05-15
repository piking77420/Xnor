%module CoreC

%ignore XnorCore::Scene::renderOctree;

%typemap(cscode) XnorCore::Scene
%{
    
%}

%include "scene/scene.hpp"
