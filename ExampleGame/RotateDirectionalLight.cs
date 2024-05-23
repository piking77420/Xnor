using Xnor.Core;

namespace Game
{
    public class RotateDirectionalLight : ScriptComponent
    {
        [Serialized]
        private float speed = 1f;
        
        [Serialized]
        Vector3 axis = Vector3.UnitY;
        
        protected override void Begin()
        {
        }

        protected override void Update()
        {
            Vector3 rot = Transform.RotationEulerAngle;
            rot.X = Time.TotalTime;
            Transform.RotationEulerAngle = rot;
        }
    }
}
