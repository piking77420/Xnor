using System;
using System.Reflection;

namespace Xnor.Core
{
    public static partial class CoreC
    {
        public static Component ComponentListGetItem(ComponentList list, uint index)
        {
            string managedTypeName = CoreCNative.NativeComponentListGetItem(ComponentList.getCPtr(list).Handle, index, out nint item);
            
            Type managedType = Assembly.GetExecutingAssembly().GetType(managedTypeName);
            if (managedType == null)
                throw new ArgumentException("Invalid type name", managedTypeName);
            
            ConstructorInfo ctor = managedType.GetConstructor(BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance, null, new[] { typeof(IntPtr), typeof(bool) }, null);
            if (ctor == null)
                throw new("Couldn't find Component constructor");

            return (Component) managedType.Assembly.CreateInstance(managedTypeName, false, BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance, null,
                new object[] { item, false }, null, null);
        }
    }
}
