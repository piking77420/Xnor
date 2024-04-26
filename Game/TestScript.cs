using System;
using Xnor.Core;

namespace Game
{
    public class TestScript : ScriptComponent
    {
        private ColorHsva color;
        private float intensity;

        private Light light;

        public TestScript()
        {
            Logger.LogTempDebug("Bobby was constructed!");
            intensity = 1f / 360f;
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
                if (!TryGetComponent(out light))
                    return;
            }
            
            // Dangerous
            // Only for limit testing
            entity.AddComponent(new PointLight());

            light.intensity += 0.1f;
            
            color.H += intensity;
            if (color.H >= 1f)
                color.H = 0f;

            light.color = (Colorf) color;

            transform.SetPositionX(MathF.Cos(Time.GetTotalTime()) * 2f);
            transform.SetPositionZ(MathF.Sin(Time.GetTotalTime()) * 2f);
        }
    }
}
