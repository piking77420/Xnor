using Xnor.Core;

namespace Game
{
    public class PlayerCamera : ScriptComponent
    {
        public Vector2 ViewAngle;

        public Entity PlayerEntity;

        public override void Begin()
        {
            PlayerEntity = World.scene.FindEntityByName("Player");
        }

        public override void Update()
        {
            if (Input.GetKey(Key.Left))
                ViewAngle.X += Time.GetDeltaTime();
            else if (Input.GetKey(Key.Right))
                ViewAngle.X -= Time.GetDeltaTime();

            if (Input.GetKey(Key.Up))
                ViewAngle.Y -= Time.GetDeltaTime();
            else if (Input.GetKey(Key.Down))
                ViewAngle.Y += Time.GetDeltaTime();

            transform.RotationEulerAngle = new(ViewAngle.Y, ViewAngle.X, 0f);
            
            transform.Position = PlayerEntity.transform.Position;
        }
    }
}
