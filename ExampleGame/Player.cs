using System;
using Xnor.Core;

namespace Game
{
    public class Player : ScriptComponent
    {
        public float MovementSpeed = 1f;
        public float JumpSpeed = 5f;

        private Camera camera;
        private CapsuleCollider collider;
        private Entity cameraEntity;

        private Vector3 movement = Vector3.Zero;
        private Vector3 velocity = Vector3.Zero;
        
        
        [Serialized] 
        private float MaxVelocity = 10f;

        
        protected override void Begin()
        {
            cameraEntity = World.scene.FindEntityByName("Camera");
            camera = cameraEntity.GetComponent<CameraComponent>().camera;
            collider = GetComponent<CapsuleCollider>();
            collider.SetFriction(1000);
        }
        
        protected override void Update()
        {
            HandleInputs();

            // Update velocity
            Vector3 desiredVelocity = movement * MovementSpeed;
            desiredVelocity.Y = velocity.Y;
            velocity = 0.75f * velocity + 0.25f * desiredVelocity;

            velocity.X = Math.Clamp(velocity.X, -MaxVelocity, MaxVelocity);
            velocity.Y = Math.Clamp(velocity.Y, -MaxVelocity, MaxVelocity);
            velocity.Z = Math.Clamp(velocity.Z, -MaxVelocity, MaxVelocity);

            // Update position
            //Transform.Position += velocity * Time.DeltaTime;
            velocity = new Vector3();
        }

        private void HandleInputs()
        {
            Vector3 cameraFront = camera.front;
            cameraFront.Y = 0f;

            Vector3 cameraRight = camera.right;
            cameraRight.Y = 0f;
            
            // Handle movement
            if (Input.GetKey(Key.W))
                movement += cameraFront;
            else if (Input.GetKey(Key.S))
                movement -= cameraFront;
            if (Input.GetKey(Key.A))
                movement -= cameraRight;
            else if (Input.GetKey(Key.D))
                movement += cameraRight;

            // Handle jump
            if (Input.GetKey(Key.Space))
                velocity.Y += JumpSpeed;
        }
    }
}
