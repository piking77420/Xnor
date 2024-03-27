namespace Xnor.Core
{
    public struct ColorRgb(byte r, byte g, byte b)
    {
        public static readonly ColorRgb White = new(byte.MaxValue, byte.MaxValue, byte.MaxValue);
        public static readonly ColorRgb Gray = new(byte.MaxValue / 2, byte.MaxValue / 2, byte.MaxValue / 2);
        public static readonly ColorRgb Black = new(0, 0, 0);
        
        public static readonly ColorRgb Red = new(byte.MaxValue, 0, 0);
        public static readonly ColorRgb Green = new(0, byte.MaxValue, 0);
        public static readonly ColorRgb Blue = new(0, 0, byte.MaxValue);
        
        public static readonly ColorRgb Yellow = new(byte.MaxValue, byte.MaxValue, 0);
        public static readonly ColorRgb Cyan = new(0, byte.MaxValue, byte.MaxValue);
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

        public byte R = r;
        public byte G = g;
        public byte B = b;

        public ColorRgb() : this(0, 0, 0) { }

        public ColorRgb(byte rgb) : this(rgb, rgb, rgb) { }
    }
}
