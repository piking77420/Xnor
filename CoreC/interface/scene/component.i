%module CoreC

%typemap(csclassmodifiers) XnorCore::Component "public abstract class";

%typemap(cscode) XnorCore::Component
%{
    public T GetComponent<T>() where T : Component { return entity.GetComponent<T>(); }
    
    public List<T> GetComponents<T>() where T : Component { return entity.GetComponents<T>(); }

    public bool TryGetComponent<T>(out T component) where T : Component { return entity.TryGetComponent<T>(out component); }

    public void RemoveComponent<T>() where T : Component { entity.RemoveComponent<T>(); }
%}

%include "scene/component.hpp"
