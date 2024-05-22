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

public static partial class CoreC {
  public static float Zero {
    get {
      float ret = CoreCNative.Zero_get();
      return ret;
    } 
  }

  public static float Pi {
    get {
      float ret = CoreCNative.Pi_get();
      return ret;
    } 
  }

  public static float PiTimes2 {
    get {
      float ret = CoreCNative.PiTimes2_get();
      return ret;
    } 
  }

  public static float PiOver2 {
    get {
      float ret = CoreCNative.PiOver2_get();
      return ret;
    } 
  }

  public static float PiOver4 {
    get {
      float ret = CoreCNative.PiOver4_get();
      return ret;
    } 
  }

  public static float Deg2Rad {
    get {
      float ret = CoreCNative.Deg2Rad_get();
      return ret;
    } 
  }

  public static float Rad2Deg {
    get {
      float ret = CoreCNative.Rad2Deg_get();
      return ret;
    } 
  }

  public static float Sign(float number) {
    float ret = CoreCNative.Sign(number);
    return ret;
  }

  public static float Abs(float number) {
    float ret = CoreCNative.Abs(number);
    return ret;
  }

  public static float Round(float number) {
    float ret = CoreCNative.Round(number);
    return ret;
  }

  public static float Floor(float number) {
    float ret = CoreCNative.Floor(number);
    return ret;
  }

  public static float Ceil(float number) {
    float ret = CoreCNative.Ceil(number);
    return ret;
  }

  public static float Modulo(float a, float b) {
    float ret = CoreCNative.Modulo(a, b);
    return ret;
  }

  public static void Approach(SWIGTYPE_p_float value, float target, float step) {
    CoreCNative.Approach(SWIGTYPE_p_float.getCPtr(value), target, step);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public static float YoYo(float value) {
    float ret = CoreCNative.YoYo(value);
    return ret;
  }

  public static bool OnInterval(float value, float lastValue, float interval) {
    bool ret = CoreCNative.OnInterval(value, lastValue, interval);
    return ret;
  }

  public static bool IsZero(float value) {
    bool ret = CoreCNative.IsZero__SWIG_0(value);
    return ret;
  }

  public static bool IsZero(float value, float zero) {
    bool ret = CoreCNative.IsZero__SWIG_1(value, zero);
    return ret;
  }

  public static float MakeZero(float value) {
    float ret = CoreCNative.MakeZero__SWIG_0(value);
    return ret;
  }

  public static float MakeZero(float value, float zero) {
    float ret = CoreCNative.MakeZero__SWIG_1(value, zero);
    return ret;
  }

  public static bool Equals(float a, float b) {
    bool ret = CoreCNative.Equals(a, b);
    return ret;
  }

  public static float SineIn(float t) {
    float ret = CoreCNative.SineIn(t);
    return ret;
  }

  public static float SineOut(float t) {
    float ret = CoreCNative.SineOut(t);
    return ret;
  }

  public static float SineInOut(float t) {
    float ret = CoreCNative.SineInOut(t);
    return ret;
  }

  public static float QuadIn(float t) {
    float ret = CoreCNative.QuadIn(t);
    return ret;
  }

  public static float QuadOut(float t) {
    float ret = CoreCNative.QuadOut(t);
    return ret;
  }

  public static float QuadInOut(float t) {
    float ret = CoreCNative.QuadInOut(t);
    return ret;
  }

  public static float CubicIn(float t) {
    float ret = CoreCNative.CubicIn(t);
    return ret;
  }

  public static float CubicOut(float t) {
    float ret = CoreCNative.CubicOut(t);
    return ret;
  }

  public static float CubicInOut(float t) {
    float ret = CoreCNative.CubicInOut(t);
    return ret;
  }

  public static float QuartIn(float t) {
    float ret = CoreCNative.QuartIn(t);
    return ret;
  }

  public static float QuartOut(float t) {
    float ret = CoreCNative.QuartOut(t);
    return ret;
  }

  public static float QuartInOut(float t) {
    float ret = CoreCNative.QuartInOut(t);
    return ret;
  }

  public static float QuintIn(float t) {
    float ret = CoreCNative.QuintIn(t);
    return ret;
  }

  public static float QuintOut(float t) {
    float ret = CoreCNative.QuintOut(t);
    return ret;
  }

  public static float QuintInOut(float t) {
    float ret = CoreCNative.QuintInOut(t);
    return ret;
  }

  public static float ExpoIn(float t) {
    float ret = CoreCNative.ExpoIn(t);
    return ret;
  }

  public static float ExpoOut(float t) {
    float ret = CoreCNative.ExpoOut(t);
    return ret;
  }

  public static float ExpoInOut(float t) {
    float ret = CoreCNative.ExpoInOut(t);
    return ret;
  }

  public static float CircIn(float t) {
    float ret = CoreCNative.CircIn(t);
    return ret;
  }

  public static float CircOut(float t) {
    float ret = CoreCNative.CircOut(t);
    return ret;
  }

  public static float CircInOut(float t) {
    float ret = CoreCNative.CircInOut(t);
    return ret;
  }

  public static float BackIn(float t) {
    float ret = CoreCNative.BackIn(t);
    return ret;
  }

  public static float BackOut(float t) {
    float ret = CoreCNative.BackOut(t);
    return ret;
  }

  public static float BackInOut(float t) {
    float ret = CoreCNative.BackInOut(t);
    return ret;
  }

  public static float ElasticIn(float t) {
    float ret = CoreCNative.ElasticIn(t);
    return ret;
  }

  public static float ElasticOut(float t) {
    float ret = CoreCNative.ElasticOut(t);
    return ret;
  }

  public static float ElasticInOut(float t) {
    float ret = CoreCNative.ElasticInOut(t);
    return ret;
  }

  public static float BounceIn(float t) {
    float ret = CoreCNative.BounceIn(t);
    return ret;
  }

  public static float BounceOut(float t) {
    float ret = CoreCNative.BounceOut(t);
    return ret;
  }

  public static float BounceInOut(float t) {
    float ret = CoreCNative.BounceInOut(t);
    return ret;
  }

  public static uint MaxSpotLights {
    get {
      uint ret = CoreCNative.MaxSpotLights_get();
      return ret;
    } 
  }

  public static uint MaxPointLights {
    get {
      uint ret = CoreCNative.MaxPointLights_get();
      return ret;
    } 
  }

  public static uint MaxDirectionalLights {
    get {
      uint ret = CoreCNative.MaxDirectionalLights_get();
      return ret;
    } 
  }

  public static uint MaxBones {
    get {
      uint ret = CoreCNative.MaxBones_get();
      return ret;
    } 
  }

  public static uint DirectionalCascadeLevel {
    get {
      uint ret = CoreCNative.DirectionalCascadeLevel_get();
      return ret;
    } 
  }

  public static uint DirectionalCascadeLevelAllocation {
    get {
      uint ret = CoreCNative.DirectionalCascadeLevelAllocation_get();
      return ret;
    } 
  }

  public static uint GamepadAxisCount {
    get {
      uint ret = CoreCNative.GamepadAxisCount_get();
      return ret;
    } 
  }

  public static uint GamepadButtonCount {
    get {
      uint ret = CoreCNative.GamepadButtonCount_get();
      return ret;
    } 
  }

  public static uint GamepadButtonStatusCount {
    get {
      uint ret = CoreCNative.GamepadButtonStatusCount_get();
      return ret;
    } 
  }

  public static uint KeyCount {
    get {
      uint ret = CoreCNative.KeyCount_get();
      return ret;
    } 
  }

  public static uint KeyStatusCount {
    get {
      uint ret = CoreCNative.KeyStatusCount_get();
      return ret;
    } 
  }

  public static uint MouseButtonCount {
    get {
      uint ret = CoreCNative.MouseButtonCount_get();
      return ret;
    } 
  }

  public static uint MouseButtonStatusCount {
    get {
      uint ret = CoreCNative.MouseButtonStatusCount_get();
      return ret;
    } 
  }

  public static void AlignImGuiCursor(float objectWidth, float alignment) {
    CoreCNative.AlignImGuiCursor__SWIG_0(objectWidth, alignment);
  }

  public static void AlignImGuiCursor(float objectWidth) {
    CoreCNative.AlignImGuiCursor__SWIG_1(objectWidth);
  }

  public static string HumanizeString(string str) {
    string ret = CoreCNative.HumanizeString(str);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public static string HumanizeVariableName(string str) {
    string ret = CoreCNative.HumanizeVariableName(str);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public static string RemoveNamespaces(string str) {
    string ret = CoreCNative.RemoveNamespaces(str);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public static float RemapValue(float oldValue,  Vector2  oldRange,  Vector2  newRange) {
    float ret = CoreCNative.RemapValue__SWIG_0(oldValue,  oldRange ,  newRange );
    return ret;
  }

  public static uint RemapValue(uint oldValue,  Vector2i  oldRange,  Vector2i  newRange) {
    uint ret = CoreCNative.RemapValue__SWIG_1(oldValue,  oldRange ,  newRange );
    return ret;
  }

  public static float NormalizeAngle(float angle) {
    float ret = CoreCNative.NormalizeAngle(angle);
    return ret;
  }

  public static  Vector3  NormalizeAngles( Vector3  angles) {
			Vector3 ret = CoreCNative.NormalizeAngles( angles );
			return ret;
		}

  public static  Vector3  GetQuaternionEulerAngles( Quaternion  rot) {
			Vector3 ret = CoreCNative.GetQuaternionEulerAngles( ref rot );
			return ret;
		}

  public static void OpenInExplorer(SWIGTYPE_p_std__filesystem__path path) {
    CoreCNative.OpenInExplorer__SWIG_0(SWIGTYPE_p_std__filesystem__path.getCPtr(path));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public static void OpenInExplorer(SWIGTYPE_p_std__filesystem__path path, bool isFile) {
    CoreCNative.OpenInExplorer__SWIG_1(SWIGTYPE_p_std__filesystem__path.getCPtr(path), isFile);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public static void OpenFile(SWIGTYPE_p_std__filesystem__path filepath) {
    CoreCNative.OpenFile(SWIGTYPE_p_std__filesystem__path.getCPtr(filepath));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public static bool StringEqualsIgnoreCase(string a, string b) {
    bool ret = CoreCNative.StringEqualsIgnoreCase(a, b);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int TerminalCommand(string command, bool asynchronous) {
    int ret = CoreCNative.TerminalCommand__SWIG_0(command, asynchronous);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public static int TerminalCommand(string command) {
    int ret = CoreCNative.TerminalCommand__SWIG_1(command);
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
    return ret;
  }

  public static void CreateEmptyFile(SWIGTYPE_p_std__filesystem__path path) {
    CoreCNative.CreateEmptyFile(SWIGTYPE_p_std__filesystem__path.getCPtr(path));
    if (CoreCNative.SWIGPendingException.Pending) throw CoreCNative.SWIGPendingException.Retrieve();
  }

  public static readonly float ZERO = CoreCNative.ZERO_get();
}

}
