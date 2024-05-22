using Xnor.Core;

namespace Game
{
    public class PlayerWeapon : ScriptComponent
    {
        private Entity parent;
        private Entity shootPoint;

        //private List<Entity> bullets = new();

        private void SetupBullet(Entity bullet)
        {
            StaticMeshRenderer meshRenderer = bullet.AddComponent<StaticMeshRenderer>();
            meshRenderer.mesh = ResourceManager.GetMesh("assets/models/sphere.obj");
        }
        
        public void Shoot()
        {
            Entity bullet = World.scene.CreateEntity("Bullet");
            SetupBullet(bullet);
            
            BoxCollider coll = bullet.AddComponent<BoxCollider>();
            Matrix shootPointMatrix = shootPoint.Transform.WorldMatrix;
            bullet.Transform.Position = new(shootPointMatrix.M30, shootPointMatrix.M31, shootPointMatrix.M32);
            coll.AddForce((parent.Transform.Position - bullet.Transform.Position).Normalized() * 10f);
        }
        
        protected override void Begin()
        {
            parent = Entity.GetParent();
            shootPoint = Entity.GetChild(0);
        }

        protected override void Update()
        {
            if (Input.GetMouseButton(MouseButton.Left, MouseButtonStatus.Pressed))
                Shoot();
        }
    }
}
