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

public class Font : Resource {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal Font(global::System.IntPtr cPtr, bool cMemoryOwn) : base(CorePINVOKE.Font_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Font obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(Font obj) {
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

  protected override void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          CorePINVOKE.delete_Font(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public static SWIGTYPE_p_std__arrayT_char_const_p_1_t FileExtensions {
    get {
      global::System.IntPtr cPtr = CorePINVOKE.Font_FileExtensions_get();
      SWIGTYPE_p_std__arrayT_char_const_p_1_t ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_std__arrayT_char_const_p_1_t(cPtr, false);
      return ret;
    } 
  }

  public Font() : this(CorePINVOKE.new_Font(), true) {
  }

}

}
