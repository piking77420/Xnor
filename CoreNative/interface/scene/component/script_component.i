%module CoreNative

%typemap(cscode) XnorCore::ScriptComponent
%{
    public Entity Entity => GetEntity();

    public Transform Transform => GetTransform();

    public T GetComponent<T>() where T : Component { return Entity.GetComponent<T>(); }
    
    public global::System.Collections.Generic.List<T> GetComponents<T>() where T : Component { return Entity.GetComponents<T>(); }

    public bool TryGetComponent<T>(out T component) where T : Component { return Entity.TryGetComponent<T>(out component); }

    public virtual void OnTriggerEnter(Collider self, Collider other, CollisionData data) {}

    public virtual void OnTriggerStay(Collider self, Collider other, CollisionData data) {}

    public virtual void OnTriggerExit(Collider self, Collider other) {}

    public virtual void OnCollisionEnter(Collider self, Collider other, CollisionData data) {}

    public virtual void OnCollisionStay(Collider self, Collider other, CollisionData data) {}

    public virtual void OnCollisionExit(Collider self, Collider other) {}
%}

%include "scene/component/script_component.hpp"
