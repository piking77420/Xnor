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
        
        [Serialized] 
        private float MaxVelocity = 10f;

        private float JectpackTim = 4f;
        
        private float JetPackValue = 2f;

        private bool CanJump = true;
        
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
            Vector3 currentVelocity = collider.GetLinearVelocity();
            Vector3 desiredVelocity = MovementSpeed * movement;
            desiredVelocity.Y = currentVelocity.Y;
            Vector3 newVelocity = 0.75f * currentVelocity + 0.25f * desiredVelocity;
            
            // Update position
            collider.SetLinearVelocity(newVelocity);
            
            movement = new Vector3();
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
            if (CanJump && Input.GetKey(Key.Space))
            {
                collider.AddForce(Vector3.UnitY * JumpSpeed);
                JetPackValue -= Time.DeltaTime;
            }

            if (JetPackValue <= 0)
            {
                CanJump = false;
                JectpackTim -= Time.DeltaTime;
                
                if (JectpackTim <= 0)
                {
                    JectpackTim = 4f;
                    CanJump = true;
                    JetPackValue = 2f;
                }
            }
        }
    }
}
