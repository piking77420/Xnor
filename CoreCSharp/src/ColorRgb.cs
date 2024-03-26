namespace Xnor.Core
{
    public struct ColorRgb(byte r, byte g, byte b)
    {
        public static readonly ColorRgb Red = new(byte.MaxValue, 0, 0);
        public static readonly ColorRgb Green = new(0, byte.MaxValue, 0);
        public static readonly ColorRgb Blue = new(0, 0, byte.MaxValue);
        
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

        public static explicit operator ColorHsva(ColorRgb color)
        {
            ColorHsva hsv = new();
            byte minVal = Math.Min(Math.Min(color.R, color.G), color.B);
            byte maxVal = Math.Max(Math.Max(color.R, color.G), color.B);
            hsv.V = maxVal;
            byte delta = (byte) (maxVal - minVal);
            if (delta == 0)
            {
                hsv.H = 0;
                hsv.S = 0;
            }
            else
            {
                hsv.S = (byte) (byte.MaxValue * delta / maxVal);
                if (color.R == maxVal)
                    hsv.H = (byte) (ColorHsva.HueAngle * (color.G - color.B) / delta);
                else if (color.G == maxVal)
                    hsv.H = (byte) (ColorHsva.HueAngle * 2 - 1 + ColorHsva.HueAngle * (color.B - color.R) / delta);
                else
                    hsv.H = (byte) (ColorHsva.HueAngle * 4 - 1 + ColorHsva.HueAngle * (color.R - color.G) / delta);
            }
            return hsv;
        }

        public static explicit operator Colorf(ColorRgb color) => new(color.R / (float) byte.MaxValue, color.G / (float) byte.MaxValue, color.B / (float) byte.MaxValue);

        public byte R = r;
        public byte G = g;
        public byte B = b;

        public ColorRgb() : this(0, 0, 0) { }

        public ColorRgb(byte rgb) : this(rgb, rgb, rgb) { }
    }
}
