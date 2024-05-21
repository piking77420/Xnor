using Xnor.Core;

namespace Game
{
    public class PlayerWeapons : ScriptComponent
    {
        private Entity parent;
        private Entity shootPoint;

        //private List<Entity> bullets = new();

        void SetBulletPrefabs(Entity bullet)
        {
            StaticMeshRenderer meshRenderer = bullet.AddComponent<StaticMeshRenderer>();
            meshRenderer.mesh = ResourceManager.GetMesh("assets/models/sphere.obj");
        }
        
        public void Shoot()
        {
            Entity bullet = World.scene.CreateEntity("Bullet ammoooo");
            SetBulletPrefabs(bullet);
            
            BoxCollider coll = bullet.AddComponent<BoxCollider>();
            bullet.Transform.Position = new(shootPoint.Transform.WorldMatrix.M30,shootPoint.Transform.WorldMatrix.M31,shootPoint.Transform.WorldMatrix.M32);
            coll.AddForce((parent.Transform.Position - bullet.Transform.Position).Normalized() * 100);
        }
        
        protected override void Begin()
        {
            parent = entity.GetParent();
            shootPoint = entity.GetChild(0);
        }

        protected override void Update()
        {
            if (Input.GetMouseButton(MouseButton.Left, MouseButtonStatus.Pressed))
                Shoot();
        }
    }
}
