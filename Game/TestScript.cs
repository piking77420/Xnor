namespace Game
{
    public class TestScript : ScriptComponent
    {
        private ColorHsva color;
        private byte intensity;
        
        /*[Obsolete]
        bool	Boolean;
        byte	Byte;
        sbyte	SByte;
        char	Char;
        double	Double;
        float	Single;
        int	    Int32;
        uint	UInt32;
        nint	IntPtr;
        nuint	UIntPtr;
        long	Int64;
        ulong	UInt64;
        short	Int16;
        ushort	UInt16;
        */

        private Light light;

        public TestScript()
        {
            /*
            Boolean = false;
            Byte = 0;
            SByte = 0;
            Char = '0';
            Double = 0;
            Single = 0;
            Int32 = 0;
            UInt32 = 0;
            IntPtr = 0;
            UIntPtr = 0;
            Int64 = 0;
            UInt64 = 0;
            Int16 = 0;
            UInt16 = 0;
            */
            Logger.LogTempDebug("Bobby was constructed!");
            intensity = 1;
        }
        
        public override void Begin()
        {
            Logger.LogTempDebug("Bobby began acting!");

            color = (ColorHsva) ColorRgb.Red;
        }

        public override void Update()
        {
            if (light == null)
            {
                light = GetComponent<PointLight>();
                return;
            }
            
            color.H += intensity;

            light.color = (Colorf) color;
            light.intensity = 200f;

            transform.SetPositionX(MathF.Cos(Time.GetTotalTime()) * 2f);
            transform.SetPositionZ(MathF.Sin(Time.GetTotalTime()) * 2f);
        }
    }
}
