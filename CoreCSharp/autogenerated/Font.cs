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

public class Font : Resource {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal Font(global::System.IntPtr cPtr, bool cMemoryOwn) : base(CoreCNative.Font_SWIGUpcast(cPtr), cMemoryOwn) {
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
          CoreCNative.delete_Font(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public static SWIGTYPE_p_std__arrayT_char_const_p_1_t FileExtensions {
    get {
      global::System.IntPtr cPtr = CoreCNative.Font_FileExtensions_get();
      SWIGTYPE_p_std__arrayT_char_const_p_1_t ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_std__arrayT_char_const_p_1_t(cPtr, false);
      return ret;
    } 
  }

  public class Character : global::System.IDisposable {
    private global::System.Runtime.InteropServices.HandleRef swigCPtr;
    protected bool swigCMemOwn;
  
    internal Character(global::System.IntPtr cPtr, bool cMemoryOwn) {
      swigCMemOwn = cMemoryOwn;
      swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
    }
  
    internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Character obj) {
      return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
    }
  
    internal static global::System.Runtime.InteropServices.HandleRef swigRelease(Character obj) {
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
  
    ~Character() {
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
            CoreCNative.delete_Font_Character(swigCPtr);
          }
          swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
        }
      }
    }
  
    public Texture texture {
      set {
        CoreCNative.Font_Character_texture_set(swigCPtr, Texture.getCPtr(value));
      } 
      get {
        global::System.IntPtr cPtr = CoreCNative.Font_Character_texture_get(swigCPtr);
        Texture ret = (cPtr == global::System.IntPtr.Zero) ? null : new Texture(cPtr, false);
        return ret;
      } 
    }
  
    public Vector2i size {
      set {
        CoreCNative.Font_Character_size_set(swigCPtr,  ref value );
      } 
  		get { 
  			nint ptr = CoreCNative.Font_Character_size_get(swigCPtr);
  			Vector2i ret = (Vector2i) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Vector2i));
  			return ret;
  		}
  	
    }
  
    public Vector2i bearing {
      set {
        CoreCNative.Font_Character_bearing_set(swigCPtr,  ref value );
      } 
  		get { 
  			nint ptr = CoreCNative.Font_Character_bearing_get(swigCPtr);
  			Vector2i ret = (Vector2i) global::System.Runtime.InteropServices.Marshal.PtrToStructure(ptr, typeof(Vector2i));
  			return ret;
  		}
  	
    }
  
    public uint advance {
      set {
        CoreCNative.Font_Character_advance_set(swigCPtr, value);
      } 
      get {
        uint ret = CoreCNative.Font_Character_advance_get(swigCPtr);
        return ret;
      } 
    }
  
    public Character() : this(CoreCNative.new_Font_Character(), true) {
    }
  
  }

  public Font(string name) : this(CoreCNative.new_Font__SWIG_0(name), true) {
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public override bool Load(SWIGTYPE_p_unsigned_char buffer, long length) {
    bool ret = CoreCNative.Font_Load__SWIG_0(swigCPtr, SWIGTYPE_p_unsigned_char.getCPtr(buffer), length);
    return ret;
  }

  public override bool Load(SWIGTYPE_p_XnorCore__PointerT_XnorCore__File_t file) {
    bool ret = CoreCNative.Font_Load__SWIG_1(swigCPtr, SWIGTYPE_p_XnorCore__PointerT_XnorCore__File_t.getCPtr(file));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public Font() : this(CoreCNative.new_Font__SWIG_1(), true) {
  }

  public override void CreateInInterface() {
    CoreCNative.Font_CreateInInterface(swigCPtr);
  }

  public override void DestroyInInterface() {
    CoreCNative.Font_DestroyInInterface(swigCPtr);
  }

  public Font.Character GetGlyphByChar(char characters) {
    Font.Character ret = new Font.Character(CoreCNative.Font_GetGlyphByChar(swigCPtr, characters), false);
    return ret;
  }

}

}
