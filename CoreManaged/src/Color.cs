using System;

namespace Xnor.Core
{
    public struct Color
    {
        public static readonly Color White = new(1f, 1f, 1f);
        public static readonly Color Gray = new(0.5f, 0.5f, 0.5f);
        public static readonly Color Black = new(0, 0, 0);
        
        public static readonly Color Red = new(1f, 0, 0);
        public static readonly Color Green = new(0, 1f, 0);
        public static readonly Color Blue = new(0, 0, 1f);
        
        public static readonly Color Yellow = new(1f, 1f, 0);
        public static readonly Color LightBlue = new(0, 1f, 1f);
        public static readonly Color Magenta = new(1f, 0, 1f);

        public static Color Lerp(Color a, Color b, float t)
        {
            return new(
                CoreNative.Lerp(a.R, b.R, t),
                CoreNative.Lerp(a.G, b.G, t),
                CoreNative.Lerp(a.B, b.B, t),
                CoreNative.Lerp(a.A, b.A, t)
            );
        }

        public static Color operator +(Color a, Color b) => new(MathF.Min(a.R + b.R, 1f), MathF.Min(a.G + b.G, 1f), MathF.Min(a.B + b.B, 1f), MathF.Min(a.A + b.A, 1f));

        public static Color operator *(Color a, Color b) => new(MathF.Min(a.R * b.R, 1f), MathF.Min(a.G * b.G, 1f), MathF.Min(a.B * b.B, 1f), MathF.Min(a.A * b.A, 1f));

        public static Color operator *(Color color, float alphaFactor) => new(color.R, color.G, color.B, MathF.Min(color.A * alphaFactor, 1f));

        public static explicit operator ColorHsva(Color color)
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
                if (CoreNative.Equals(color.R, maxVal))
                    hsv.H = ColorHsva.HueCircleOver6 * (color.G - color.B) * deltaInverse;
                else if (CoreNative.Equals(color.G, maxVal))
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

        public static explicit operator Vector3(Color color) => new(color.R, color.G, color.B);

        public static explicit operator Vector4(Color color) => new(color.R, color.G, color.B, color.A);

        public static bool operator ==(Color left, Color right) => left.Equals(right);

        public static bool operator !=(Color left, Color right) => !left.Equals(right);
        
        public float R;
        public float G;
        public float B;
        public float A;

        public Color(float r, float g, float b, float a = 1f)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public Color(float rgb) : this(rgb, rgb, rgb) { }
        
        public override string ToString() => $"R: {R}, G: {G}, B: {B}, A: {A}";
    
        public bool Equals(Color other) => CoreNative.Equals(R, other.R) && CoreNative.Equals(G, other.G) && CoreNative.Equals(B, other.B) && CoreNative.Equals(A, other.A);

        public override bool Equals(object obj) => obj is Color other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(R, G, B, A);
    }
}
