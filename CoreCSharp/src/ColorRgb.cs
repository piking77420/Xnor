using System;

namespace Xnor.Core
{
    public struct ColorRgb
    {
        public static readonly ColorRgb White = new(byte.MaxValue, byte.MaxValue, byte.MaxValue);
        public static readonly ColorRgb Gray = new(byte.MaxValue / 2, byte.MaxValue / 2, byte.MaxValue / 2);
        public static readonly ColorRgb Black = new(0, 0, 0);
        
        public static readonly ColorRgb Red = new(byte.MaxValue, 0, 0);
        public static readonly ColorRgb Green = new(0, byte.MaxValue, 0);
        public static readonly ColorRgb Blue = new(0, 0, byte.MaxValue);
        
        public static readonly ColorRgb Yellow = new(byte.MaxValue, byte.MaxValue, 0);
        public static readonly ColorRgb LightBlue = new(0, byte.MaxValue, byte.MaxValue);
        public static readonly ColorRgb Magenta = new(byte.MaxValue, 0, byte.MaxValue);
        
        public static ColorRgb operator +(ColorRgb a, ColorRgb b)
        {
            // Prevent overflow
            int newR = a.R + b.R;
            int newG = a.G + b.G;
            int newB = a.B + b.B;
            
            return new(
                (byte) Math.Min(newR, byte.MaxValue),
                (byte) Math.Min(newG, byte.MaxValue),
                (byte) Math.Min(newB, byte.MaxValue)
            );
        }

        public static ColorRgb operator *(ColorRgb a, ColorRgb b)
        {
            // Prevent overflow
            int newR = a.R * b.R;
            int newG = a.G * b.G;
            int newB = a.B * b.B;
            
            return new(
                (byte) Math.Min(newR, byte.MaxValue),
                (byte) Math.Min(newG, byte.MaxValue),
                (byte) Math.Min(newB, byte.MaxValue)
            );
        }

        public static implicit operator ColorRgba(ColorRgb color) => new(color.R, color.G, color.B);

        public static explicit operator ColorHsva(ColorRgb color) => (ColorHsva) (ColorRgba) color;

        public static explicit operator Colorf(ColorRgb color) => new(color.R / (float) byte.MaxValue, color.G / (float) byte.MaxValue, color.B / (float) byte.MaxValue);

        public byte R;
        public byte G;
        public byte B;

        public ColorRgb(byte r, byte g, byte b)
        {
            R = r;
            G = g;
            B = b;
        }

        public ColorRgb(byte rgb) : this(rgb, rgb, rgb) { }

        public override string ToString() => $"R: {R}, G: {G}, B: {B}";
    
        public bool Equals(ColorRgb other) => CoreC.Equals(R, other.R) && CoreC.Equals(G, other.G) && CoreC.Equals(B, other.B);

        public override bool Equals(object obj) => obj is ColorRgb other && Equals(other);

        public override int GetHashCode() => HashCode.Combine(R, G, B);

        public static bool operator ==(ColorRgb left, ColorRgb right) => left.Equals(right);

        public static bool operator !=(ColorRgb left, ColorRgb right) => !left.Equals(right);
    }
}
