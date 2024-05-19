using Xnor.Core;

namespace Game
{
    public class Lantern : ScriptComponent
    {
        public float MaxRotationAngle = CoreNative.PiOver4;
        
        private Transform rotationOrigin;

        [Serialized]
        private float baseRotationX;

        protected override void Begin()
        {
            for (uint i = 0; i < Entity.GetChildCount(); i++)
            {
                Entity child = Entity.GetChild(i);
                if (child.name != "RotationOrigin")
                    continue;
                
                rotationOrigin = child.Transform;
                break;
            }

            baseRotationX = Transform.RotationX;
        }

        protected override void Update()
        {
            float time = Time.TotalTime * 0.25f;
            Transform.RotationX = baseRotationX + CoreNative.YoYo(time % 1f) * MaxRotationAngle * 2f - MaxRotationAngle;
        }
    }
}
