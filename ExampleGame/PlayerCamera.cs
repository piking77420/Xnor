﻿using Xnor.Core;

namespace Game
{
    public class PlayerCamera : ScriptComponent
    {
        public Vector2 ViewAngle;

        private Entity playerEntity;

        private const float MaxPitch = 89f; 
        
        protected override void Begin()
        {
            playerEntity = World.scene.FindEntityByName("Player");
            Transform.Rotation = playerEntity.Transform.Rotation;
        }

        protected override void Update()
        {
            Vector2 mousDelta = Input.GetMouseDelta();
            
            ViewAngle.X -= mousDelta.X * Time.DeltaTime;
            ViewAngle.Y += mousDelta.Y * Time.DeltaTime;
            
            if (ViewAngle.Y * CoreNative.Rad2Deg > MaxPitch)
                ViewAngle.Y = MaxPitch * CoreNative.Deg2Rad;
            if (ViewAngle.Y * CoreNative.Rad2Deg < -MaxPitch)
                ViewAngle.Y = -MaxPitch * CoreNative.Deg2Rad;
            
            //Transform.RotationEulerAngle = new(ViewAngle.Y, ViewAngle.X, 0f);
            //Transform.Position = playerEntity.Transform.Position;

        }
    }
}
