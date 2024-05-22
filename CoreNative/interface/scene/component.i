%module CoreNative

%typemap(csclassmodifiers) XnorCore::Component "public abstract class";

%typemap(csdisposing, methodname="Dispose", methodmodifiers="protected", parameters="bool disposing") XnorCore::Component
%{
    {
        lock(this)
        {
            if (swigCPtr.Handle != global::System.IntPtr.Zero)
            {
                /*if (!disposing)
                    entity.GetComponents().Remove(this);*/
                
                if (swigCMemOwn)
                {
                    swigCMemOwn = false;
                    CoreNativeFuncs.delete_Component(swigCPtr);
                }
                swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
            }
        }
    }
%}

%csmethodmodifiers XnorCore::Component::Begin "protected virtual";
%csmethodmodifiers XnorCore::Component::Update "protected virtual";

%rename(Entity) XnorCore::Component::entity;

%include "scene/component.hpp"
