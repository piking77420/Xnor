%module CoreC

%typemap(cscode) XnorCore::ScriptComponent
%{
    public Entity entity => GetEntity();

    public Transform transform => GetTransform();

    public T GetComponent<T>() where T : Component { return entity.GetComponent<T>(); }
    
    public global::System.Collections.Generic.List<T> GetComponents<T>() where T : Component { return entity.GetComponents<T>(); }

    public bool TryGetComponent<T>(out T component) where T : Component { return entity.TryGetComponent<T>(out component); }
%}

%include "scene/component/script_component.hpp"
