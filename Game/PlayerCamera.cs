﻿using Xnor.Core;

namespace Game
{
    public class PlayerCamera : ScriptComponent
    {
        public Vector2 ViewAngle;

        public Entity PlayerEntity;

        protected override void Begin()
        {
            PlayerEntity = World.scene.FindEntityByName("Player");
        }

        protected override void Update()
        {
            if (Input.GetKey(Key.Left))
                ViewAngle.X += Time.DeltaTime;
            else if (Input.GetKey(Key.Right))
                ViewAngle.X -= Time.DeltaTime;

            if (Input.GetKey(Key.Up))
                ViewAngle.Y -= Time.DeltaTime;
            else if (Input.GetKey(Key.Down))
                ViewAngle.Y += Time.DeltaTime;

            Transform.RotationEulerAngle = new(ViewAngle.Y, ViewAngle.X, 0f);
            
            Transform.Position = PlayerEntity.Transform.Position;
        }
    }
}
