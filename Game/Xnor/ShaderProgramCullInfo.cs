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

public class ShaderProgramCullInfo : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ShaderProgramCullInfo(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ShaderProgramCullInfo obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(ShaderProgramCullInfo obj) {
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

  ~ShaderProgramCullInfo() {
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
          CorePINVOKE.delete_ShaderProgramCullInfo(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public bool enableCullFace {
    set {
      CorePINVOKE.ShaderProgramCullInfo_enableCullFace_set(swigCPtr, value);
    } 
    get {
      bool ret = CorePINVOKE.ShaderProgramCullInfo_enableCullFace_get(swigCPtr);
      return ret;
    } 
  }

  public CullFace cullFace {
    set {
      CorePINVOKE.ShaderProgramCullInfo_cullFace_set(swigCPtr, (int)value);
    } 
    get {
      CullFace ret = (CullFace)CorePINVOKE.ShaderProgramCullInfo_cullFace_get(swigCPtr);
      return ret;
    } 
  }

  public FrontFace frontFace {
    set {
      CorePINVOKE.ShaderProgramCullInfo_frontFace_set(swigCPtr, (int)value);
    } 
    get {
      FrontFace ret = (FrontFace)CorePINVOKE.ShaderProgramCullInfo_frontFace_get(swigCPtr);
      return ret;
    } 
  }

  public ShaderProgramCullInfo() : this(CorePINVOKE.new_ShaderProgramCullInfo(), true) {
  }

}

}
