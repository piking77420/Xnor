using System.Runtime.InteropServices;

namespace Xnor.Core
{
    internal partial class CoreCNative
    {
        protected class StringHelper {

            public delegate string StringDelegate(string message);
            static StringDelegate stringDelegate = new StringDelegate(CreateString);

            [DllImport("CoreC", EntryPoint="RegisterStringCallback_Core")]
            public static extern void SWIGRegisterStringCallback_Core(StringDelegate stringDelegate);

            static string CreateString(string cString)
            {
                return cString;
            }

            static StringHelper()
            {
                SWIGRegisterStringCallback_Core(stringDelegate);
            }
        }

        private static StringHelper stringHelper = new StringHelper();

        [DllImport("CoreC")]
        public static extern string NativeComponentListGetItem(nint componentList, uint index, out nint item);
    }
}
