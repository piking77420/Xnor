namespace Xnor.Core
{
    public struct Colorf(float r, float g, float b, float a = 1f)
    {
        public static readonly Colorf White = new(1f, 1f, 1f);
        public static readonly Colorf Gray = new(0.5f, 0.5f, 0.5f);
        public static readonly Colorf Black = new(0, 0, 0);
        
        public static readonly Colorf Red = new(1f, 0, 0);
        public static readonly Colorf Green = new(0, 1f, 0);
        public static readonly Colorf Blue = new(0, 0, 1f);
        
        public static readonly Colorf Yellow = new(1f, 1f, 0);
        public static readonly Colorf LightBlue = new(0, 1f, 1f);
        public static readonly Colorf Magenta = new(1f, 0, 1f);
        
        public static Colorf operator +(Colorf a, Colorf b) => new(MathF.Min(a.R + b.R, 1f), MathF.Min(a.G + b.G, 1f), MathF.Min(a.B + b.B, 1f), MathF.Min(a.A + b.A, 1f));

        public static Colorf operator *(Colorf a, Colorf b) => new(MathF.Min(a.R * b.R, 1f), MathF.Min(a.G * b.G, 1f), MathF.Min(a.B * b.B, 1f), MathF.Min(a.A * b.A, 1f));

        public static Colorf operator *(Colorf color, float alphaFactor) => new(color.R, color.G, color.B, MathF.Min(color.A * alphaFactor, 1f));

        public static explicit operator ColorRgb(Colorf color) => new((byte) MathF.Round(color.R * byte.MaxValue, MidpointRounding.AwayFromZero), (byte) MathF.Round(color.G * byte.MaxValue, MidpointRounding.AwayFromZero), (byte) MathF.Round(color.B * byte.MaxValue, MidpointRounding.AwayFromZero));

        public static explicit operator ColorRgba(Colorf color) => new((byte) MathF.Round(color.R * byte.MaxValue, MidpointRounding.AwayFromZero), (byte) MathF.Round(color.G * byte.MaxValue, MidpointRounding.AwayFromZero), (byte) MathF.Round(color.B * byte.MaxValue, MidpointRounding.AwayFromZero), (byte) MathF.Round(color.A * byte.MaxValue, MidpointRounding.AwayFromZero));

        public static explicit operator ColorHsva(Colorf color)
        {
            ColorHsva hsv = new()
            {
                A = color.A
            };
            float minVal = MathF.Min(MathF.Min(color.R, color.G), color.B);
            float maxVal = MathF.Max(MathF.Max(color.R, color.G), color.B);
            hsv.V = maxVal;
            float delta = maxVal - minVal;
    
            if (delta == 0) // Black
            {
                hsv.H = 0;
                hsv.S = 0;
            }
            else
            {
                hsv.S = delta / maxVal;
                float deltaInverse = 1f / delta;
                if (CoreC.Equals(color.R, maxVal))
                    hsv.H = ColorHsva.HueCircleOver6 * (color.G - color.B) * deltaInverse;
                else if (CoreC.Equals(color.G, maxVal))
                    hsv.H = ColorHsva.HueCircleOver3 + ColorHsva.HueCircleOver6 * (color.B - color.R) * deltaInverse;
                else
                    hsv.H = ColorHsva.HueCircleOver3 * 2 + ColorHsva.HueCircleOver6 * (color.R - color.G) * deltaInverse;

                float floor = MathF.Floor(hsv.H);
                if (hsv.H < 0f)
                    hsv.H += MathF.Abs(floor);
                else
                    hsv.H -= floor;
            }
            return hsv;
        }

        public static explicit operator Vector3(Colorf color) => new(color.R, color.G, color.B);

        public static explicit operator Vector4(Colorf color) => new(color.R, color.G, color.B, color.A);
        
        public float R = r;
        public float G = g;
        public float B = b;
        public float A = a;

        public Colorf() : this(0f, 0f, 0f) { }

        public Colorf(float rgb) : this(rgb, rgb, rgb) { }
        
        public override string ToString() => $"R: {R}, G: {G}, B: {B}, A: {A}";
    
        public bool Equals(Colorf other) => CoreC.Equals(R, other.R) && CoreC.Equals(G, other.G) && CoreC.Equals(B, other.B) && CoreC.Equals(A, other.A);

        public override bool Equals(object obj) => obj is Colorf other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(R, G, B, A);

        public static bool operator ==(Colorf left, Colorf right) => left.Equals(right);

        public static bool operator !=(Colorf left, Colorf right) => !left.Equals(right);
    }
}
