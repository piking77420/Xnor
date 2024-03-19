using System.Reflection;

namespace Xnor.Core
{
    public static partial class Core
    {
        public static Component ComponentListGetItem(ComponentList list, uint index)
        {
            string managedTypeName = CoreNative.NativeComponentListGetItem(ComponentList.getCPtr(list).Handle, index, out nint item);
            
            Type managedType = Assembly.GetExecutingAssembly().GetType(managedTypeName);
            if (managedType == null)
                throw new ArgumentException("Invalid type name", managedTypeName);
            
            ConstructorInfo ctor = managedType.GetConstructor(BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance, [typeof(IntPtr), typeof(bool)]);
            if (ctor == null)
                throw new Exception("Couldn't find Component constructor");

            return (Component) managedType.Assembly.CreateInstance(managedTypeName, false, BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance, null,
                [item, false], null, null);
        }
    }
}
