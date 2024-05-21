using System;
using Xnor.Core;

namespace Game
{
    public class Lantern : ScriptComponent
    {
        public float MaxRotationAngle = CoreNative.PiOver4;

        private Vector3 startingPosition;

        protected override void Begin()
        {
            startingPosition = Transform.Position;
        }

        protected override void Update()
        {
            float time = Time.TotalTime * 0.25f;
            Transform.Position = startingPosition + new Vector3(MathF.Cos(time), MathF.Sin(time), 0f);
        }
    }
}
