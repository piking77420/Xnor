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

public class CameraUniformData : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal CameraUniformData(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(CameraUniformData obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(CameraUniformData obj) {
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

  ~CameraUniformData() {
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
          CoreCNative.delete_CameraUniformData(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public Matrix view {
    set {
      CoreCNative.CameraUniformData_view_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.CameraUniformData_view_get(swigCPtr);
			Matrix ret = (Matrix) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Matrix));
			return ret;
		}
	
  }

  public Matrix projection {
    set {
      CoreCNative.CameraUniformData_projection_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.CameraUniformData_projection_get(swigCPtr);
			Matrix ret = (Matrix) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Matrix));
			return ret;
		}
	
  }

  public Matrix invView {
    set {
      CoreCNative.CameraUniformData_invView_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.CameraUniformData_invView_get(swigCPtr);
			Matrix ret = (Matrix) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Matrix));
			return ret;
		}
	
  }

  public Matrix invProjection {
    set {
      CoreCNative.CameraUniformData_invProjection_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.CameraUniformData_invProjection_get(swigCPtr);
			Matrix ret = (Matrix) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Matrix));
			return ret;
		}
	
  }

  public Vector3 cameraPos {
    set {
      CoreCNative.CameraUniformData_cameraPos_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.CameraUniformData_cameraPos_get(swigCPtr);
			Vector3 ret = (Vector3) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Vector3));
			return ret;
		}
	
  }

  public float near {
    set {
      CoreCNative.CameraUniformData_near_set(swigCPtr, value);
    } 
    get {
      float ret = CoreCNative.CameraUniformData_near_get(swigCPtr);
      return ret;
    } 
  }

  public float far {
    set {
      CoreCNative.CameraUniformData_far_set(swigCPtr, value);
    } 
    get {
      float ret = CoreCNative.CameraUniformData_far_get(swigCPtr);
      return ret;
    } 
  }

  public CameraUniformData() : this(CoreCNative.new_CameraUniformData(), true) {
  }

}

}
