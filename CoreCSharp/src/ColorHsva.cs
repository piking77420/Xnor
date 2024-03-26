namespace Xnor.Core
{
    public struct ColorHsva(byte h, byte s, byte v, byte a = byte.MaxValue)
    {
        public const byte HueAngle = 43;

        public static ColorHsva operator *(ColorHsva color, float alphaFactor) => new(color.H, color.S, color.V, (byte) MathF.Min(color.A * alphaFactor, byte.MaxValue));

        public static explicit operator ColorRgb(ColorHsva color)
        {
            byte hi = (byte) (color.H / HueAngle);
            byte f = (byte) (color.H % HueAngle * 6);
            byte p = (byte) ((color.V * (byte.MaxValue - color.S) + sbyte.MaxValue) / byte.MaxValue);
            byte q = (byte) ((color.V * (byte.MaxValue - (color.S * f + sbyte.MaxValue) / byte.MaxValue) + sbyte.MaxValue) / byte.MaxValue);
            byte t = (byte) ((color.V * (byte.MaxValue - (color.S * (byte.MaxValue - f) + sbyte.MaxValue) / byte.MaxValue) + sbyte.MaxValue) / byte.MaxValue);

            return hi switch
            {
                0 => new(color.V, t, p),
                1 => new(q, color.V, p),
                2 => new(p, color.V, t),
                3 => new(p, q, color.V),
                4 => new(t, p, color.V),
                _ => new(color.V, p, q)
            };
        }

        public static explicit operator ColorRgba(ColorHsva color)
        {
            byte hi = (byte) (color.H / HueAngle);
            byte f = (byte) (color.H % HueAngle * 6);
            byte p = (byte) ((color.V * (byte.MaxValue - color.S) + sbyte.MaxValue) / byte.MaxValue);
            byte q = (byte) ((color.V * (byte.MaxValue - (color.S * f + sbyte.MaxValue) / byte.MaxValue) + sbyte.MaxValue) / byte.MaxValue);
            byte t = (byte) ((color.V * (byte.MaxValue - (color.S * (byte.MaxValue - f) + sbyte.MaxValue) / byte.MaxValue) + sbyte.MaxValue) / byte.MaxValue);

            return hi switch
            {
                0 => new(color.V, t, p, color.A),
                1 => new(q, color.V, p, color.A),
                2 => new(p, color.V, t, color.A),
                3 => new(p, q, color.V, color.A),
                4 => new(t, p, color.V, color.A),
                _ => new(color.V, p, q, color.A)
            };
        }

        public static explicit operator Colorf(ColorHsva color)
        {
            byte hi = (byte) (color.H / HueAngle);
            byte f = (byte) (color.H % HueAngle * 6);
            byte p = (byte) ((color.V * (byte.MaxValue - color.S) + sbyte.MaxValue) / byte.MaxValue);
            byte q = (byte) ((color.V * (byte.MaxValue - (color.S * f + sbyte.MaxValue) / byte.MaxValue) + sbyte.MaxValue) / byte.MaxValue);
            byte t = (byte) ((color.V * (byte.MaxValue - (color.S * (byte.MaxValue - f) + sbyte.MaxValue) / byte.MaxValue) + sbyte.MaxValue) / byte.MaxValue);

            return (Colorf) (hi switch
            {
                // Do not remove ColorRgba after new
                // We need this because otherwise the C# compiler would consider it as a new Colorf instead
                0 => new ColorRgba(color.V, t, p, color.A),
                1 => new ColorRgba(q, color.V, p, color.A),
                2 => new ColorRgba(p, color.V, t, color.A),
                3 => new ColorRgba(p, q, color.V, color.A),
                4 => new ColorRgba(t, p, color.V, color.A),
                _ => new ColorRgba(color.V, p, q, color.A)
            });
        }
        
        public byte H = h;
        public byte S = s;
        public byte V = v;
        public byte A = a;

        public ColorHsva() : this(0, 0, 0) { }
    }
}
