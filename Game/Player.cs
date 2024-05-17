using Xnor.Core;

namespace Game
{
    public class Player : ScriptComponent
    {
        public float MovementSpeed = 3f;
        public Entity CameraEntity;

        private Camera camera;
        private CapsuleCollider collider;
        
        protected override void Begin()
        {
            CameraEntity = World.scene.FindEntityByName("Camera");
            camera = CameraEntity.GetComponent<CameraComponent>().camera;
            
            collider = GetComponent<CapsuleCollider>();
        }

        protected override void Update()
        {
            Vector3 movement = Vector3.Zero;

            Vector3 cameraFront = camera.front;
            cameraFront.Y = 0f;

            Vector3 cameraRight = camera.right;
            cameraRight.Y = 0f;
            
            if (Input.GetKey(Key.W))
                movement += cameraFront;
            else if (Input.GetKey(Key.S))
                movement -= cameraFront;
            if (Input.GetKey(Key.A))
                movement += cameraRight;
            else if (Input.GetKey(Key.D))
                movement -= cameraRight;

            Logger.LogInfo($"q : {movement * MovementSpeed * Time.DeltaTime}");
            collider.AddForce(movement * MovementSpeed * Time.DeltaTime);
        }
    }
}
