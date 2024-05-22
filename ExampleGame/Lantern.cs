using System;
using System.Runtime.InteropServices;
using Xnor.Core;

namespace Game
{
    public class Lantern : ScriptComponent
    {
        public float MaxRotationAngle = CoreNative.PiOver4;

        private Vector3 startingPosition;

        public float RotationSpeed = 1f;

        protected override void Begin()
        {
            startingPosition = Transform.Position;
        }

        protected override void Update()
        {
            float time = Time.TotalTime * RotationSpeed;
            //Transform.RotationEulerAngleZ = MathF.Sin(time) / 2f;
        }
    }
}
