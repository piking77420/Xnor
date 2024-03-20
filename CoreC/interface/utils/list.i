%module CoreC

%typemap(csinterfaces) XnorCore::List<XnorCore::Component*>
%{
    global::System.Collections.Generic.IEnumerable<global::Xnor.Core.Component>
%}

%typemap(cscode) XnorCore::List<XnorCore::Component*>
%{
    private class ListEnumerator : global::System.Collections.Generic.IEnumerator<Component>
    {
        private readonly ComponentList list;
        private uint index;
        private Component current;
        
        public ListEnumerator(ComponentList list)
        {
            this.list = list;
            Reset();
        }
        
        public bool MoveNext()
        {
            if (index >= list.GetSize())
            {
                current = null;
                return false;
            }

            current = list[index++];

            return true;
        }

        public void Reset()
        {
            index = 0;
            current = null;
        }

        public Component Current { get => current; }

        object global::System.Collections.IEnumerator.Current => Current;

        public void Dispose() { }
    }

    public global::System.Collections.Generic.IEnumerator<Component> GetEnumerator()
    {
        return new ListEnumerator(this);
    }

    global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }

    public Component this[uint index]
    {
        get => CoreC.ComponentListGetItem(this, index);
    }
%}

%include "utils/list.hpp"

%template(ComponentList) XnorCore::List<XnorCore::Component*>;
