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

public class ComponentList : 
    global::System.Collections.Generic.IEnumerable<global::Xnor.Core.Component>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ComponentList(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ComponentList obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(ComponentList obj) {
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

  ~ComponentList() {
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
          CoreCNative.delete_ComponentList(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

    private class ListEnumerator : global::System.Collections.Generic.IEnumerator<Component>
    {
        private readonly ComponentList list;
        private uint index;
        private Component current;
        
        public ListEnumerator(ComponentList list)
        {
            this.list = list;
            Reset();
        }
        
        public bool MoveNext()
        {
            if (index >= list.GetSize())
            {
                current = null;
                return false;
            }

            current = list[index++];

            return true;
        }

        public void Reset()
        {
            index = 0;
            current = null;
        }

        public Component Current { get => current; }

        object global::System.Collections.IEnumerator.Current => Current;

        public void Dispose() { }
    }

    public global::System.Collections.Generic.IEnumerator<Component> GetEnumerator()
    {
        return new ListEnumerator(this);
    }

    global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }

    public Component this[uint index]
    {
        get => CoreC.ComponentListGetItem(this, index);
    }

    public void ForEach(global::System.Action<Component> action)
    {
        foreach (Component c in this)
            action(c);
    }

    public Component Find(global::System.Predicate<Component> predicate)
    {
        foreach (Component c in this)
        {
            if (predicate(c))
                return c;
        }

        return null;
    }

    public bool Exists(global::System.Predicate<Component> predicate)
    {
        foreach (Component c in this)
        {
            if (predicate(c))
                return true;
        }

        return false;
    }

  public bool Empty() {
    bool ret = CoreCNative.ComponentList_Empty(swigCPtr);
    return ret;
  }

  public void Add(Component element) {
    CoreCNative.ComponentList_Add(swigCPtr, Component.getCPtr(element));
  }

  public void Remove(Component element) {
    CoreCNative.ComponentList_Remove(swigCPtr, Component.getCPtr(element));
  }

  public uint FindPosition(SWIGTYPE_p_std__functionT_bool_fXnorCore__Component_pconst_pF_t lambda) {
    uint ret = CoreCNative.ComponentList_FindPosition(swigCPtr, SWIGTYPE_p_std__functionT_bool_fXnorCore__Component_pconst_pF_t.getCPtr(lambda));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint GetSize() {
    uint ret = CoreCNative.ComponentList_GetSize(swigCPtr);
    return ret;
  }

  public uint GetCapacity() {
    uint ret = CoreCNative.ComponentList_GetCapacity(swigCPtr);
    return ret;
  }

}

}
