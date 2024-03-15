//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.2.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Xnor {

public class Skybox : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Skybox(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Skybox obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(Skybox obj) {
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

  ~Skybox() {
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
          CorePINVOKE.delete_Skybox(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public void LoadCubeMap(SWIGTYPE_p_std__arrayT_std__string_6_t cubeMapFiles) {
    CorePINVOKE.Skybox_LoadCubeMap(swigCPtr, SWIGTYPE_p_std__arrayT_std__string_6_t.getCPtr(cubeMapFiles));
    if (CorePINVOKE.SWIGPendingException.Pending) throw CorePINVOKE.SWIGPendingException.Retrieve();
  }

  public Cubemap GetCubeMap() {
    global::System.IntPtr cPtr = CorePINVOKE.Skybox_GetCubeMap__SWIG_0(swigCPtr);
    Cubemap ret = (cPtr == global::System.IntPtr.Zero) ? null : new Cubemap(cPtr, false);
    return ret;
  }

  public Skybox() : this(CorePINVOKE.new_Skybox(), true) {
  }

}

}
