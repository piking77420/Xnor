using System;

namespace Xnor.Core
{
    public struct ColorRgba
    {
        public static readonly ColorRgba White = new(byte.MaxValue, byte.MaxValue, byte.MaxValue);
        public static readonly ColorRgba Gray = new(byte.MaxValue / 2, byte.MaxValue / 2, byte.MaxValue / 2);
        public static readonly ColorRgba Black = new(0, 0, 0);
        
        public static readonly ColorRgba Red = new(byte.MaxValue, 0, 0);
        public static readonly ColorRgba Green = new(0, byte.MaxValue, 0);
        public static readonly ColorRgba Blue = new(0, 0, byte.MaxValue);
        
        public static readonly ColorRgba Yellow = new(byte.MaxValue, byte.MaxValue, 0);
        public static readonly ColorRgba LightBlue = new(0, byte.MaxValue, byte.MaxValue);
        public static readonly ColorRgba Magenta = new(byte.MaxValue, 0, byte.MaxValue);
        
        public static ColorRgba operator +(ColorRgba a, ColorRgba b)
        {
            // Prevent overflow
            int newR = a.R + b.R;
            int newG = a.G + b.G;
            int newB = a.B + b.B;
            int newA = a.A + b.A;
            
            return new(
                (byte) Math.Min(newR, byte.MaxValue),
                (byte) Math.Min(newG, byte.MaxValue),
                (byte) Math.Min(newB, byte.MaxValue),
                (byte) Math.Min(newA, byte.MaxValue)
            );
        }

        public static ColorRgba operator *(ColorRgba a, ColorRgba b)
        {
            // Prevent overflow
            int newR = a.R * b.R;
            int newG = a.G * b.G;
            int newB = a.B * b.B;
            int newA = a.A * b.A;
            
            return new(
                (byte) Math.Min(newR, byte.MaxValue),
                (byte) Math.Min(newG, byte.MaxValue),
                (byte) Math.Min(newB, byte.MaxValue),
                (byte) Math.Min(newA, byte.MaxValue)
            );
        }

        public static ColorRgba operator *(ColorRgba color, float alphaFactor) => new(color.R, color.G, color.B, (byte) MathF.Min(color.A * alphaFactor, byte.MaxValue));

        public static explicit operator ColorRgb(ColorRgba color) => new(color.R, color.G, color.B);

        public static explicit operator ColorHsva(ColorRgba color) => (ColorHsva) (Colorf) color;

        public static explicit operator Colorf(ColorRgba color) => new(color.R / (float) byte.MaxValue, color.G / (float) byte.MaxValue, color.B / (float) byte.MaxValue, color.A / (float) byte.MaxValue);
        
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        public ColorRgba(byte r, byte g, byte b, byte a = byte.MaxValue)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public ColorRgba(byte rgb) : this(rgb, rgb, rgb) { }
        
        public override string ToString() => $"R: {R}, G: {G}, B: {B}, A: {A}";
    
        public bool Equals(ColorRgba other) => CoreC.Equals(R, other.R) && CoreC.Equals(G, other.G) && CoreC.Equals(B, other.B) && CoreC.Equals(A, other.A);

        public override bool Equals(object obj) => obj is ColorRgba other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(R, G, B, A);

        public static bool operator ==(ColorRgba left, ColorRgba right) => left.Equals(right);

        public static bool operator !=(ColorRgba left, ColorRgba right) => !left.Equals(right);
    }
}
