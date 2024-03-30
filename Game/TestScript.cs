namespace Game
{
    public class TestScript : ScriptComponent
    {
        private ColorHsva color;
        private byte intensity;

        private Light light;

        public TestScript()
        {
            Logger.LogTempDebug("Bobby was constructed!");
            intensity = 1;
        }

        ~TestScript()
        {
            Logger.LogTempDebug("Bobby was destructed!");
        }
        
        public override void Begin()
        {
            Logger.LogTempDebug("Bobby began acting!");

            color = ColorHsva.Red;
        }

        public override void Update()
        {
            if (light == null)
            {
                if (!TryGetComponent(out PointLight pointLight))
                    return;

                light = pointLight;
            }
            
            color.H += intensity;

            light.color = (Colorf) color;

            transform.SetPositionX(MathF.Cos(Time.GetTotalTime()) * 2f);
            transform.SetPositionZ(MathF.Sin(Time.GetTotalTime()) * 2f);
        }
    }
}
