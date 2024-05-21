using System.Collections.Generic;
using Xnor.Core;


namespace Game
{
    public class PlayerWeapons : ScriptComponent
    {
        private Entity parent;
        private Entity shootPoint;

        private List<Entity> bullets = new List<Entity>();
        public void Shoot()
        {
            return;
            Entity bullet = World.scene.CreateEntity("Bullet ammoooo");
            bullet.AddComponent<BoxCollider>();
            bullet.AddComponent<StaticMeshRenderer>();
            bullets.Add(bullet);
            
            
            var coll = bullet.GetComponent<BoxCollider>();
            bullet.Transform.Position = new Vector3(shootPoint.Transform.WorldMatrix.M03,shootPoint.Transform.WorldMatrix.M03,shootPoint.Transform.WorldMatrix.M23);
            coll.AddForce((parent.Transform.Position - bullet.Transform.Position).Normalized() * 100);
        }
        
        protected override void Begin()
        {
            parent = entity.GetParent();
            shootPoint = entity.GetChild(0);
        }

        protected override void Update()
        {
            if (Input.GetMouseButton(MouseButton.Left))
            {
                Shoot();
            }

            foreach (var VARIABLE in bullets)
            {
                
            }
        }

    }
}