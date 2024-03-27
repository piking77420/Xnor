namespace Xnor.Core
{
    public struct ColorHsva(byte h, byte s, byte v, byte a = byte.MaxValue)
    {
        public const byte HueCircle = byte.MaxValue;
        public const float HueCircleOver3 = 1f / 3f * HueCircle;
        public const float HueCircleOver6 = 1f / 6f * HueCircle;
        
        public static readonly ColorHsva White = new(0, 0, byte.MaxValue);
        public static readonly ColorHsva Gray = new(0, 0, byte.MaxValue / 2);
        public static readonly ColorHsva Black = new(0, 0, 0);
        
        public static readonly ColorHsva Red = new(0, byte.MaxValue, byte.MaxValue);
        public static readonly ColorHsva Green = new((byte) HueCircleOver3, byte.MaxValue, byte.MaxValue);
        public static readonly ColorHsva Blue = new((byte) (HueCircleOver3 * 2), byte.MaxValue, byte.MaxValue);
        
        public static readonly ColorHsva Yellow = new((byte) HueCircleOver6, byte.MaxValue, byte.MaxValue);
        public static readonly ColorHsva Cyan = new((byte) (HueCircleOver6 * 3), byte.MaxValue, byte.MaxValue);
        public static readonly ColorHsva Magenta = new((byte) (HueCircleOver6 * 5), byte.MaxValue, byte.MaxValue);

        public static ColorHsva operator *(ColorHsva color, float alphaFactor) => new(color.H, color.S, color.V, (byte) MathF.Min(color.A * alphaFactor, byte.MaxValue));

        public static explicit operator ColorRgb(ColorHsva color) => (ColorRgb) (ColorRgba) color;

        public static explicit operator ColorRgba(ColorHsva color)
        {
            byte hi = (byte) (color.H / HueCircleOver6);
            byte f = (byte) (color.H % HueCircleOver6 * 6);
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

        public static explicit operator Colorf(ColorHsva color) => (Colorf) (ColorRgba) color;
        
        public byte H = h;
        public byte S = s;
        public byte V = v;
        public byte A = a;

        public ColorHsva() : this(0, 0, 0) { }
    }
}
