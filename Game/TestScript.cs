namespace Game
{
    public class TestScript : ScriptComponent
    {
        private ColorHsva color;

        public TestScript()
        {
            Logger.LogTempDebug("Bobby is alive!");
        }
        
        public override void Begin()
        {
            color = (ColorHsva) ColorRgb.Red();
        }

        public override void Update()
        {
            color.h++;
        }
    }
}
