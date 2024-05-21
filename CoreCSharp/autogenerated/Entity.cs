//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.2.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Xnor.Core {

public class Entity : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Entity(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Entity obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(Entity obj) {
    if (obj != null) {
      if (!obj.swigCMemOwn)
        throw new global::System.ApplicationException("Cannot release ownership as memory is not owned");
      global::System.Runtime.InteropServices.HandleRef ptr = obj.swigCPtr;
      obj.swigCMemOwn = false;
      obj.Dispose();
      return ptr;
    } else {
      return new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
    }
  }

  ~Entity() {
    Dispose(false);
  }

  public void Dispose() {
    Dispose(true);
    global::System.GC.SuppressFinalize(this);
  }

  protected virtual void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          CoreCNative.delete_Entity(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

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

  public Entity() : this(CoreCNative.new_Entity(), true) {
  }

  public Transform transform {
    set {
      CoreCNative.Entity_transform_set(swigCPtr, Transform.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = CoreCNative.Entity_transform_get(swigCPtr);
      Transform ret = (cPtr == global::System.IntPtr.Zero) ? null : new Transform(cPtr, false);
      return ret;
    } 
  }

  public string name {
    set {
      CoreCNative.Entity_name_set(swigCPtr, value);
      if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    } 
    get {
      string ret = CoreCNative.Entity_name_get(swigCPtr);
      if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public void AddComponent(Component component) {
    CoreCNative.Entity_AddComponent(swigCPtr, Component.getCPtr(component));
  }

  public Guid GetGuid() {
    Guid ret = new Guid(CoreCNative.Entity_GetGuid(swigCPtr), false);
    return ret;
  }

  public Entity GetParent() {
    global::System.IntPtr cPtr = CoreCNative.Entity_GetParent(swigCPtr);
    Entity ret = (cPtr == global::System.IntPtr.Zero) ? null : new Entity(cPtr, false);
    return ret;
  }

  public bool HasParent() {
    bool ret = CoreCNative.Entity_HasParent(swigCPtr);
    return ret;
  }

  public Entity GetChild(uint index) {
    global::System.IntPtr cPtr = CoreCNative.Entity_GetChild(swigCPtr, index);
    Entity ret = (cPtr == global::System.IntPtr.Zero) ? null : new Entity(cPtr, false);
    return ret;
  }

  public uint GetChildCount() {
    uint ret = CoreCNative.Entity_GetChildCount(swigCPtr);
    return ret;
  }

  public bool HasChildren() {
    bool ret = CoreCNative.Entity_HasChildren(swigCPtr);
    return ret;
  }

  public bool IsParentOf(Entity child) {
    bool ret = CoreCNative.Entity_IsParentOf(swigCPtr, Entity.getCPtr(child));
    return ret;
  }

  public void SetParent(Entity parent) {
    CoreCNative.Entity_SetParent(swigCPtr, Entity.getCPtr(parent));
  }

  public void AddChild(Entity child) {
    CoreCNative.Entity_AddChild(swigCPtr, Entity.getCPtr(child));
  }

  public void RemoveChild(Entity child) {
    CoreCNative.Entity_RemoveChild(swigCPtr, Entity.getCPtr(child));
  }

  public void Begin() {
    CoreCNative.Entity_Begin(swigCPtr);
  }

  public void Update() {
    CoreCNative.Entity_Update(swigCPtr);
  }

  public void PrePhysics() {
    CoreCNative.Entity_PrePhysics(swigCPtr);
  }

  public void PostPhysics() {
    CoreCNative.Entity_PostPhysics(swigCPtr);
  }

  public bool OperatorEq(Entity entity) {
    bool ret = CoreCNative.Entity_OperatorEq(swigCPtr, Entity.getCPtr(entity));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public ComponentList GetComponents() {
    ComponentList ret = new ComponentList(CoreCNative.Entity_GetComponents(swigCPtr), false);
    return ret;
  }

}

}
