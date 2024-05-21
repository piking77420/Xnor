using Xnor.Core;

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
      
            
            if (Input.GetKey(Key.Left))
                ViewAngle.X += Time.DeltaTime;
            else if (Input.GetKey(Key.Right))
                ViewAngle.X -= Time.DeltaTime;

            if (Input.GetKey(Key.Up))
                ViewAngle.Y -= Time.DeltaTime;
            else if (Input.GetKey(Key.Down))
                ViewAngle.Y += Time.DeltaTime;
            
          /*  
            if (ViewAngle.Y * calc > MaxPitch)
                ViewAngle.Y * calc = MaxPitch;
            if (ViewAngle.Y < * calc -MaxPitch)
                ViewAngle.Y = * calc -MaxPitch;
*/

            Transform.RotationEulerAngle = new(ViewAngle.Y, ViewAngle.X, 0f);
            Transform.Position = playerEntity.Transform.Position;

        }
    }
}
