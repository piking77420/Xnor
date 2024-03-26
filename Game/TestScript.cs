namespace Game
{
    public class TestScript : ScriptComponent
    {
        private ColorHsva color;

        private Light light;

        public TestScript()
        {
            Logger.LogTempDebug("Bobby was constructed!");
        }
        
        public override void Begin()
        {
            Logger.LogTempDebug("Bobby began acting!");
            
            color = (ColorHsva) ColorRgb.Red();
        }

        public override void Update()
        {
            if (light == null)
            {
                light = GetComponent<PointLight>();
                return;
            }
            
            color.h++;

            light.color = (Colorf) color;
            light.intensity = 200f;

            transform.SetPositionX(MathF.Cos(Time.GetTotalTime()) * 2f);
            transform.SetPositionZ(MathF.Sin(Time.GetTotalTime()) * 2f);
        }
    }
}
