using Xnor.Core;

namespace Game
{
    public class Lantern : ScriptComponent
    {
        private Light light;
        private Color color;

        public float Speed = 0.05f;

        protected override void Begin()
        {
            light = GetComponent<Light>();
        }

        protected override void Update()
        {
            color = Color.Lerp(color, Color.Red, Speed);

            if (Input.GetMouseButton(MouseButton.Left, MouseButtonStatus.Pressed))
                color = Color.Blue;
            if (Input.GetMouseButton(MouseButton.Right, MouseButtonStatus.Pressed))
                color = Color.Green;

            light.color = color;
        }
    }
}
