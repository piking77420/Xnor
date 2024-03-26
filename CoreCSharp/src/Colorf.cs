namespace Xnor.Core
{
    public struct Colorf(float r, float g, float b, float a = 1f)
    {
        public static Colorf operator +(Colorf a, Colorf b) => new(MathF.Min(a.R + b.R, 1f), MathF.Min(a.G + b.G, 1f), MathF.Min(a.B + b.B, 1f), MathF.Min(a.A + b.A, 1f));

        public static Colorf operator *(Colorf a, Colorf b) => new(MathF.Min(a.R * b.R, 1f), MathF.Min(a.G * b.G, 1f), MathF.Min(a.B * b.B, 1f), MathF.Min(a.A * b.A, 1f));

        public static Colorf operator *(Colorf color, float alphaFactor) => new(color.R, color.G, color.B, MathF.Min(color.A * alphaFactor, 1f));

        public static explicit operator ColorRgb(Colorf color) => new((byte) (color.R * byte.MaxValue), (byte) (color.G * byte.MaxValue), (byte) (color.B * byte.MaxValue));

        public static explicit operator ColorRgba(Colorf color) => new((byte) (color.R * byte.MaxValue), (byte) (color.G * byte.MaxValue), (byte) (color.B * byte.MaxValue), (byte) (color.A * byte.MaxValue));

        public static explicit operator ColorHsva(Colorf color) => (ColorHsva) (ColorRgba) color;

        public static explicit operator Vector3(Colorf color) => new(color.R, color.G, color.B);

        public static explicit operator Vector4(Colorf color) => new(color.R, color.G, color.B, color.A);
        
        public float R = r;
        public float G = g;
        public float B = b;
        public float A = a;

        public Colorf() : this(0f, 0f, 0f) { }

        public Colorf(float rgb) : this(rgb, rgb, rgb) { }
    }
}
