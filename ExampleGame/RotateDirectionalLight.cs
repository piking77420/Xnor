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
            Quaternion q = Quaternion.FromEuler(axis);

            Transform.Rotation *= q;
        }
    }
}
