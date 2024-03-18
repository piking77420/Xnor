%module Core

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
%}

%include "scene/entity.hpp"
