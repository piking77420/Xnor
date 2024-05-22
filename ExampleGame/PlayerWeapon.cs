using Xnor.Core;

namespace Game
{
    public class PlayerWeapon : ScriptComponent
    {
        private Entity parent;
        private Entity shootPoint;

        //private List<Entity> bullets = new();

        private float bulletSize = 0.2f;

        private MeshPointer spherePointer;
        
        private void SetupBullet(Entity bullet)
        {
            StaticMeshRenderer meshRenderer = bullet.AddComponent<StaticMeshRenderer>();
            meshRenderer.mesh = spherePointer;
        }
        
        public void Shoot()
        {
            
            Entity bullet = World.scene.CreateEntity("Bullet");
            SetupBullet(bullet);
            SphereCollider coll = bullet.AddComponent<SphereCollider>();
            coll.radius = bulletSize;
                
            Matrix shootPointMatrix = shootPoint.Transform.WorldMatrix;
            bullet.Transform.Position = new(shootPointMatrix.M30, shootPointMatrix.M31, shootPointMatrix.M32);
            bullet.Transform.Scale *= bulletSize;
            
            //Vector3 force = (bullet.Transform.Position - parent.Transform.Position).Normalized() * 5f;
            //coll.AddForce(force);

        }
        
        protected override void Begin()
        {
            parent = Entity.GetParent();
            shootPoint = Entity.GetChild(0);
            spherePointer = ResourceManager.GetMesh("assets/models/sphere.obj");
            //_boxCollider = GetComponent<BoxCollider>();
        }

        protected override void Update()
        {
            if (Input.GetMouseButton(MouseButton.Right, MouseButtonStatus.Pressed))
                Shoot();
        }
    }
}
