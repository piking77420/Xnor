%module CoreC

%typemap(cscode) XnorCore::ScriptComponent
%{
    public Entity Entity => GetEntity();

    public Transform Transform => GetTransform();

    public T GetComponent<T>() where T : Component { return Entity.GetComponent<T>(); }
    
    public global::System.Collections.Generic.List<T> GetComponents<T>() where T : Component { return Entity.GetComponents<T>(); }

    public bool TryGetComponent<T>(out T component) where T : Component { return Entity.TryGetComponent<T>(out component); }
%}

%include "scene/component/script_component.hpp"
