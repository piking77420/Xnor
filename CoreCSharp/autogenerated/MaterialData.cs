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

public class MaterialData : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal MaterialData(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MaterialData obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(MaterialData obj) {
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

  ~MaterialData() {
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
          CoreCNative.delete_MaterialData(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public Vector3 albedoColor {
    set {
      CoreCNative.MaterialData_albedoColor_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.MaterialData_albedoColor_get(swigCPtr);
			Vector3 ret = (Vector3) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Vector3));
			return ret;
		}
	
  }

  public int hasAlbedoMap {
    set {
      CoreCNative.MaterialData_hasAlbedoMap_set(swigCPtr, value);
    } 
    get {
      int ret = CoreCNative.MaterialData_hasAlbedoMap_get(swigCPtr);
      return ret;
    } 
  }

  public Vector3 emissiveColor {
    set {
      CoreCNative.MaterialData_emissiveColor_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.MaterialData_emissiveColor_get(swigCPtr);
			Vector3 ret = (Vector3) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Vector3));
			return ret;
		}
	
  }

  public float emissive {
    set {
      CoreCNative.MaterialData_emissive_set(swigCPtr, value);
    } 
    get {
      float ret = CoreCNative.MaterialData_emissive_get(swigCPtr);
      return ret;
    } 
  }

  public int hasMetallicMap {
    set {
      CoreCNative.MaterialData_hasMetallicMap_set(swigCPtr, value);
    } 
    get {
      int ret = CoreCNative.MaterialData_hasMetallicMap_get(swigCPtr);
      return ret;
    } 
  }

  public float metallic {
    set {
      CoreCNative.MaterialData_metallic_set(swigCPtr, value);
    } 
    get {
      float ret = CoreCNative.MaterialData_metallic_get(swigCPtr);
      return ret;
    } 
  }

  public int hasRoughnessMap {
    set {
      CoreCNative.MaterialData_hasRoughnessMap_set(swigCPtr, value);
    } 
    get {
      int ret = CoreCNative.MaterialData_hasRoughnessMap_get(swigCPtr);
      return ret;
    } 
  }

  public float roughness {
    set {
      CoreCNative.MaterialData_roughness_set(swigCPtr, value);
    } 
    get {
      float ret = CoreCNative.MaterialData_roughness_get(swigCPtr);
      return ret;
    } 
  }

  public int hasAmbientOcclusionMap {
    set {
      CoreCNative.MaterialData_hasAmbientOcclusionMap_set(swigCPtr, value);
    } 
    get {
      int ret = CoreCNative.MaterialData_hasAmbientOcclusionMap_get(swigCPtr);
      return ret;
    } 
  }

  public float ambientOcclusion {
    set {
      CoreCNative.MaterialData_ambientOcclusion_set(swigCPtr, value);
    } 
    get {
      float ret = CoreCNative.MaterialData_ambientOcclusion_get(swigCPtr);
      return ret;
    } 
  }

  public int hasNormalMap {
    set {
      CoreCNative.MaterialData_hasNormalMap_set(swigCPtr, value);
    } 
    get {
      int ret = CoreCNative.MaterialData_hasNormalMap_get(swigCPtr);
      return ret;
    } 
  }

  public float reflectance {
    set {
      CoreCNative.MaterialData_reflectance_set(swigCPtr, value);
    } 
    get {
      float ret = CoreCNative.MaterialData_reflectance_get(swigCPtr);
      return ret;
    } 
  }

  public MaterialData() : this(CoreCNative.new_MaterialData(), true) {
  }

}

}
