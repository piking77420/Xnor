%module CoreNative

%typemap(cscode) XnorCore::Entity
%{
    public T GetComponent<T>() where T : Component
    {
        if (TryGetComponent<T>(out T result))
            return result;

        return null;
    }
    
    public global::System.Collections.Generic.List<T> GetComponents<T>() where T : Component
    {
        global::System.Collections.Generic.List<T> result = new();

        foreach (Component component in GetComponents())
        {
            if (component is T t)
                result.Add(t);
        }

        return result;
    }

    public bool TryGetComponent<T>(out T component) where T : Component
    {
        foreach (Component comp in GetComponents())
        {
            if (comp is T t)
            {
                component = t;
                return true;
            }
        }

        component = null;
        return false;
    }

    public void AddComponent<T>() where T : Component, new() => GetComponents().Add(new T());

    public void RemoveComponent<T>() where T : Component
    {
        foreach (Component component in GetComponents())
        {
            if (component is T t)
            {
                GetComponents().Remove(t);
                return;
            }
        }
    }
    
    public bool Equals(Entity other) => OperatorEq(other);

    public override bool Equals(object obj) => obj is Entity other && Equals(other);

    public override int GetHashCode() => throw new global::System.NotImplementedException("Cannot get the hash code of a native object");

    public static bool operator ==(Entity left, Entity right) => left.Equals(right);

    public static bool operator !=(Entity left, Entity right) => !left.Equals(right);
%}

%csmethodmodifiers XnorCore::Entity::operator== "private";

%rename XnorCore::Entity::transform Transform;

%include "scene/entity.hpp"
