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

public class TextureCreateInfo : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal TextureCreateInfo(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(TextureCreateInfo obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(TextureCreateInfo obj) {
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

  ~TextureCreateInfo() {
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
          CoreCNative.delete_TextureCreateInfo(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public TextureType textureType {
    set {
      CoreCNative.TextureCreateInfo_textureType_set(swigCPtr, (int)value);
    } 
    get {
      TextureType ret = (TextureType)CoreCNative.TextureCreateInfo_textureType_get(swigCPtr);
      return ret;
    } 
  }

  public SWIGTYPE_p_std__vectorT_void_p_t datas {
    set {
      CoreCNative.TextureCreateInfo_datas_set(swigCPtr, SWIGTYPE_p_std__vectorT_void_p_t.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = CoreCNative.TextureCreateInfo_datas_get(swigCPtr);
      SWIGTYPE_p_std__vectorT_void_p_t ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_std__vectorT_void_p_t(cPtr, false);
      return ret;
    } 
  }

  public uint mipMaplevel {
    set {
      CoreCNative.TextureCreateInfo_mipMaplevel_set(swigCPtr, value);
    } 
    get {
      uint ret = CoreCNative.TextureCreateInfo_mipMaplevel_get(swigCPtr);
      return ret;
    } 
  }

  public uint depth {
    set {
      CoreCNative.TextureCreateInfo_depth_set(swigCPtr, value);
    } 
    get {
      uint ret = CoreCNative.TextureCreateInfo_depth_get(swigCPtr);
      return ret;
    } 
  }

  public Vector2i size {
    set {
      CoreCNative.TextureCreateInfo_size_set(swigCPtr,  ref value );
    } 
		get { 
			nint ptr = CoreCNative.TextureCreateInfo_size_get(swigCPtr);
			Vector2i ret = (Vector2i) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Vector2i));
			return ret;
		}
	
  }

  public TextureFiltering filtering {
    set {
      CoreCNative.TextureCreateInfo_filtering_set(swigCPtr, (int)value);
    } 
    get {
      TextureFiltering ret = (TextureFiltering)CoreCNative.TextureCreateInfo_filtering_get(swigCPtr);
      return ret;
    } 
  }

  public TextureWrapping wrapping {
    set {
      CoreCNative.TextureCreateInfo_wrapping_set(swigCPtr, (int)value);
    } 
    get {
      TextureWrapping ret = (TextureWrapping)CoreCNative.TextureCreateInfo_wrapping_get(swigCPtr);
      return ret;
    } 
  }

  public TextureFormat format {
    set {
      CoreCNative.TextureCreateInfo_format_set(swigCPtr, (int)value);
    } 
    get {
      TextureFormat ret = (TextureFormat)CoreCNative.TextureCreateInfo_format_get(swigCPtr);
      return ret;
    } 
  }

  public TextureInternalFormat internalFormat {
    set {
      CoreCNative.TextureCreateInfo_internalFormat_set(swigCPtr, (int)value);
    } 
    get {
      TextureInternalFormat ret = (TextureInternalFormat)CoreCNative.TextureCreateInfo_internalFormat_get(swigCPtr);
      return ret;
    } 
  }

  public DataType dataType {
    set {
      CoreCNative.TextureCreateInfo_dataType_set(swigCPtr, (int)value);
    } 
    get {
      DataType ret = (DataType)CoreCNative.TextureCreateInfo_dataType_get(swigCPtr);
      return ret;
    } 
  }

  public SWIGTYPE_p_float borderColor {
    set {
      CoreCNative.TextureCreateInfo_borderColor_set(swigCPtr, SWIGTYPE_p_float.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = CoreCNative.TextureCreateInfo_borderColor_get(swigCPtr);
      SWIGTYPE_p_float ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_float(cPtr, false);
      return ret;
    } 
  }

  public TextureCreateInfo() : this(CoreCNative.new_TextureCreateInfo(), true) {
  }

}

}
