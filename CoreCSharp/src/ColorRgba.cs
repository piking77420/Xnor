namespace Xnor.Core
{
    public struct ColorRgba(byte r, byte g, byte b, byte a = byte.MaxValue)
    {
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

        public static explicit operator ColorHsva(ColorRgba color)
        {
            ColorHsva hsv = new()
            {
                A = color.A
            };
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

        public static explicit operator Colorf(ColorRgba color) => new(color.R / (float) byte.MaxValue, color.G / (float) byte.MaxValue, color.B / (float) byte.MaxValue, color.A / (float) byte.MaxValue);
        
        public byte R = r;
        public byte G = g;
        public byte B = b;
        public byte A = a;

        public ColorRgba() : this(0, 0, 0) { }

        public ColorRgba(byte rgb) : this(rgb, rgb, rgb) { }
    }
}
