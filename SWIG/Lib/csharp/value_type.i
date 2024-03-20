// Value type mapping
//
// SWIG wraps all C++ classes and structs as C# classes, which are reference
// types. However, it's ideal for small C++ UDTs to be wrapped as C# structs
// (i.e. value types). As value types, they won't be subject to the C# garbage
// collection trickiness that wrapped reference types are.

%define VALUE_TYPE_TYPEMAP(CTYPE, CSTYPE) //, DELETE_FUNCTION

//%ignore CSTYPE;

%typemap(ctype, out="void*")  CTYPE, const CTYPE&, CTYPE* "CTYPE"
%typemap(imtype, out="IntPtr") CTYPE, const CTYPE&, CTYPE* 
"[MarshalAs(UnmanagedType.Struct)]CSTYPE"
%typemap(cstype) CTYPE, const CTYPE&, CTYPE* "CSTYPE"

%typemap(ctype, out="void*")  CTYPE& "CTYPE&"
%typemap(imtype, out="IntPtr") CTYPE& "ref CSTYPE"
%typemap(cstype) CTYPE& "ref CSTYPE"

%typemap(in) CTYPE %{ $1 = $input; %}
%typemap(in) CTYPE&, const CTYPE&, CTYPE* %{ $1 = &$input; %}
%typemap(out) CTYPE %{ $result = new CTYPE($1); %}
%typemap(out) CTYPE* %{ $result = new CTYPE(*$1); %}

%typemap(csin) CTYPE, const CTYPE&, CTYPE* "$csinput"
%typemap(csin) CTYPE& "ref $csinput"
%typemap(csout, excode=SWIGEXCODE) CTYPE, const CTYPE&, CTYPE&, CTYPE*
{
    IntPtr ret = $imcall;$excode
    if (ret != IntPtr.Zero)
    {
        CSTYPE jret = (CSTYPE)Marshal.PtrToStructure(ret, typeof(CSTYPE));
        //DELETE_FUNCTION(ret);
        return jret;
    }
    else
        return new CSTYPE();
}

%typemap(csvarin, excode=SWIGEXCODE2) CTYPE*
%{
    set
    {
        $imcall;$excode
    }
%}
%typemap(csvarout, excode=SWIGEXCODE2) CTYPE*
%{
    get
    {
        IntPtr ret = $imcall;$excode
        if (ret != IntPtr.Zero)
        {
            CSTYPE jret = (CSTYPE)Marshal.PtrToStructure(ret, typeof(CSTYPE));
            //DELETE_FUNCTION(ret);
            return jret;
        }
        else
            return new CSTYPE();
    }
%}

%enddef

// Example
// So, let's say I have a C++ class called MyClass in namespace MyNamespace 
// that I wanted to swig as a C# struct MyStruct in a SWIG module called 
// MyModule. Here's what I'd add to my SWIG .i file:

/*
%module MyModule

// Some module stuff here

VALUE_TYPE_TYPEMAP(MyNamespace::MyClass, MyStruct, MyModulePINVOKE.MyNamespace_MyClass_delete);

// MyNamespace::MyClass forwarding functions

%inline
%{
    void MyNamespace_MyClass_delete(void* ptr)
    {
        if (ptr != NULL)
            delete (MyNamespace::MyClass*)ptr;
    }

    // Any other forwarded functions would go here - see the link at the start of the post for more info
%}
*/
