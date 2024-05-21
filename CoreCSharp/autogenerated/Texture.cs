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

public class Texture : Resource {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal Texture(global::System.IntPtr cPtr, bool cMemoryOwn) : base(CoreCNative.Texture_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Texture obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(Texture obj) {
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
          CoreCNative.delete_Texture(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public class LoadOptions : global::System.IDisposable {
    private global::System.Runtime.InteropServices.HandleRef swigCPtr;
    protected bool swigCMemOwn;
  
    internal LoadOptions(global::System.IntPtr cPtr, bool cMemoryOwn) {
      swigCMemOwn = cMemoryOwn;
      swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
    }
  
    internal static global::System.Runtime.InteropServices.HandleRef getCPtr(LoadOptions obj) {
      return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
    }
  
    internal static global::System.Runtime.InteropServices.HandleRef swigRelease(LoadOptions obj) {
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
  
    ~LoadOptions() {
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
            CoreCNative.delete_Texture_LoadOptions(swigCPtr);
          }
          swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
        }
      }
    }
  
    public int desiredChannels {
      set {
        CoreCNative.Texture_LoadOptions_desiredChannels_set(swigCPtr, value);
      } 
      get {
        int ret = CoreCNative.Texture_LoadOptions_desiredChannels_get(swigCPtr);
        return ret;
      } 
    }
  
    public bool flipVertically {
      set {
        CoreCNative.Texture_LoadOptions_flipVertically_set(swigCPtr, value);
      } 
      get {
        bool ret = CoreCNative.Texture_LoadOptions_flipVertically_get(swigCPtr);
        return ret;
      } 
    }
  
    public LoadOptions() : this(CoreCNative.new_Texture_LoadOptions(), true) {
    }
  
  }

  public static Texture.LoadOptions defaultLoadOptions {
    set {
      CoreCNative.Texture_defaultLoadOptions_set(Texture.LoadOptions.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = CoreCNative.Texture_defaultLoadOptions_get();
      Texture.LoadOptions ret = (cPtr == global::System.IntPtr.Zero) ? null : new Texture.LoadOptions(cPtr, false);
      return ret;
    } 
  }

  public static SWIGTYPE_p_std__arrayT_char_const_p_11_t FileExtensions {
    get {
      global::System.IntPtr cPtr = CoreCNative.Texture_FileExtensions_get();
      SWIGTYPE_p_std__arrayT_char_const_p_11_t ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_std__arrayT_char_const_p_11_t(cPtr, false);
      return ret;
    } 
  }

  public Texture.LoadOptions loadData {
    set {
      CoreCNative.Texture_loadData_set(swigCPtr, Texture.LoadOptions.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = CoreCNative.Texture_loadData_get(swigCPtr);
      Texture.LoadOptions ret = (cPtr == global::System.IntPtr.Zero) ? null : new Texture.LoadOptions(cPtr, false);
      return ret;
    } 
  }

  public Texture() : this(CoreCNative.new_Texture__SWIG_0(), true) {
  }

  public Texture(string name) : this(CoreCNative.new_Texture__SWIG_1(name), true) {
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public override bool Load(SWIGTYPE_p_XnorCore__PointerT_XnorCore__File_t file) {
    bool ret = CoreCNative.Texture_Load__SWIG_0(swigCPtr, SWIGTYPE_p_XnorCore__PointerT_XnorCore__File_t.getCPtr(file));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public Texture(TextureCreateInfo createInfo) : this(CoreCNative.new_Texture__SWIG_2(TextureCreateInfo.getCPtr(createInfo)), true) {
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public Texture(TextureInternalFormat textureInternalFormat,  Vector2i  size, TextureFormat textureFormat) : this(CoreCNative.new_Texture__SWIG_3((int)textureInternalFormat,  size , (int)textureFormat), true) {
  }

  public Texture(TextureInternalFormat textureInternalFormat,  Vector2i  size) : this(CoreCNative.new_Texture__SWIG_4((int)textureInternalFormat,  size ), true) {
  }

  public override bool Load(SWIGTYPE_p_unsigned_char buffer, long length) {
    bool ret = CoreCNative.Texture_Load__SWIG_1(swigCPtr, SWIGTYPE_p_unsigned_char.getCPtr(buffer), length);
    return ret;
  }

  public override void CreateInRhi() {
    CoreCNative.Texture_CreateInRhi(swigCPtr);
  }

  public override void DestroyInRhi() {
    CoreCNative.Texture_DestroyInRhi(swigCPtr);
  }

  public override void Unload() {
    CoreCNative.Texture_Unload(swigCPtr);
  }

  public void SetIsEmbedded() {
    CoreCNative.Texture_SetIsEmbedded(swigCPtr);
  }

  public  Vector2i  GetSize() {
			Vector2i ret = CoreCNative.Texture_GetSize(swigCPtr);
			return ret;
		}

  public int GetDataChannels() {
    int ret = CoreCNative.Texture_GetDataChannels(swigCPtr);
    return ret;
  }

  public int GetChannels() {
    int ret = CoreCNative.Texture_GetChannels(swigCPtr);
    return ret;
  }

  public void BindTexture(uint index) {
    CoreCNative.Texture_BindTexture(swigCPtr, index);
  }

  public void UnbindTexture(uint index) {
    CoreCNative.Texture_UnbindTexture(swigCPtr, index);
  }

  public uint GetId() {
    uint ret = CoreCNative.Texture_GetId(swigCPtr);
    return ret;
  }

  public TextureFiltering GetTextureFiltering() {
    TextureFiltering ret = (TextureFiltering)CoreCNative.Texture_GetTextureFiltering(swigCPtr);
    return ret;
  }

  public TextureWrapping GetTextureWrapping() {
    TextureWrapping ret = (TextureWrapping)CoreCNative.Texture_GetTextureWrapping(swigCPtr);
    return ret;
  }

  public TextureInternalFormat GetInternalFormat() {
    TextureInternalFormat ret = (TextureInternalFormat)CoreCNative.Texture_GetInternalFormat(swigCPtr);
    return ret;
  }

  public TextureFormat GetTextureFormat() {
    TextureFormat ret = (TextureFormat)CoreCNative.Texture_GetTextureFormat(swigCPtr);
    return ret;
  }

}

}
