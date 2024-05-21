using Xnor.Core;

namespace Game
{
    public class Enemy : ScriptComponent
    {
        [Serialized]
        private AnimationPointer animationIdle;
        [Serialized]
        private AnimationPointer animationRun;
        [Serialized]
        private AnimationPointer animationAttack;
        
        protected override void Begin()
        {
            
        }
        
        protected override void Update()
        {
            
        }
    }
}