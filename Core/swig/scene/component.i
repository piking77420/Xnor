%module Core

%typemap(cscode) Component
%{
    T GetComponent<T>() where T : Component { return entity.GetComponent<T>(); }
    
    List<T> GetComponents<T>() where T : Component { return entity.GetComponents<T>(); }

    bool_t TryGetComponent<T>(out T component) where T : Component { return entity.TryGetComponent<T>(out component); }

    void RemoveComponent<T>() where T : Component { entity.RemoveComponent<T>(); }
%}

%include "scene/component.hpp"
