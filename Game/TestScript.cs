namespace Game
{
    public class TestScript : ScriptComponent
    {
        private ColorHsva color;
        
        public override void Begin()
        {
            base.Begin();

            color = (ColorHsva) ColorRgb.Red();

            color = Core.OperatorMul(color, 0.5f);
            
            color *= 0.5f;
            
            Console.Write("hey {0}", 2);
            Logger.Log(Logger.LogLevel.Info, "Log from {0}!", "C#");
            Logger.LogInfo("LogInfo from {0}!", "C#");
            Logger.LogTempDebug("Temporary debug log");

            Vector2 v = new(1, 3);
            v.OperatorIndex(0);

            v = Core.OperatorMul(v, 2f);

            Vector3 v2 = (Vector3) v;

            v *= 2f;

            Guid g = Guid.New();
            Logger.LogInfo("GUID: {0}", g);
        }

        public override void Update()
        {
            base.Update();

            color.h++;
        }
    }
}
