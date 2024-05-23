%module CoreNative

%ignore XnorCore::Scene::renderOctree;
%ignore XnorCore::Scene::onDestroyEntity;
%ignore XnorCore::Scene::onCreateEntity;

%typemap(cscode) XnorCore::Scene
%{
    
%}

%include "scene/scene.hpp"
