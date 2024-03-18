%module Core

%typemap(cscode) Entity
%{
    T GetComponent<T>() where T : Component
    {

    }
    
    List<T> GetComponents<T>() where T : Component
    {

    }

    bool_t TryGetComponent<T>(out T component) where T : Component
    {

    }

    void RemoveComponent<T>() where T : Component
    {

    }
%}

%include "scene/entity.hpp"
