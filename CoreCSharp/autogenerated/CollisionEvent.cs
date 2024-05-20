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

public class CollisionEvent : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal CollisionEvent(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(CollisionEvent obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(CollisionEvent obj) {
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

  ~CollisionEvent() {
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
          CoreCNative.delete_CollisionEvent(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public void Invoke(Collider args1, Collider args2, CollisionData args3) {
    CoreCNative.CollisionEvent_Invoke(swigCPtr, Collider.getCPtr(args1), Collider.getCPtr(args2), CollisionData.getCPtr(args3));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public void Clear() {
    CoreCNative.CollisionEvent_Clear(swigCPtr);
  }

  public CollisionEvent OperatorAddAssign(SWIGTYPE_p_std__functionT_void_t func) {
    CollisionEvent ret = new CollisionEvent(CoreCNative.CollisionEvent_OperatorAddAssign(swigCPtr, SWIGTYPE_p_std__functionT_void_t.getCPtr(func)), false);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public CollisionEvent OperatorSubAssign(SWIGTYPE_p_std__functionT_void_t func) {
    CollisionEvent ret = new CollisionEvent(CoreCNative.CollisionEvent_OperatorSubAssign(swigCPtr, SWIGTYPE_p_std__functionT_void_t.getCPtr(func)), false);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public CollisionEvent() : this(CoreCNative.new_CollisionEvent(), true) {
  }

}

}
