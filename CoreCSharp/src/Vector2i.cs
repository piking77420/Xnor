namespace Xnor.Core
{
    public struct Vector2i(int x, int y)
    {
        public static readonly Vector2i Zero = new();
        
        public static readonly Vector2i UnitX = new(1, 0);
        
        public static readonly Vector2i UnitY = new(0, 1);
        
        public static readonly Vector2i One = new(1);

        public static Vector2i operator+(Vector2i a, Vector2i b) => new(a.X + b.X, a.Y + b.Y);

        public static Vector2i operator-(Vector2i m) => new(-m.X, -m.Y);

        public static Vector2i operator-(Vector2i a, Vector2i b) => a + -b;

        public static Vector2i operator*(Vector2i a, Vector2i b) => new(a.X * b.X, a.Y * b.Y);

        public static Vector2i operator*(Vector2i a, int scalar) => new(a.X * scalar, a.Y * scalar);

        public static Vector2i operator*(int scalar, Vector2i b) => b * scalar;

        public static bool operator==(Vector2i a, Vector2i b) => a.Equals(b);

        public static bool operator!=(Vector2i a, Vector2i b) => !(a == b);

        public static explicit operator Vector2(Vector2i v) => new(v.X, v.Y);

        public static explicit operator Vector3(Vector2i v) => new(v.X, v.Y, 0f);

        public static explicit operator Vector4(Vector2i v) => new(v.X, v.Y, 0f, 1f);
        
        public int X = x;
        public int Y = y;

        public Vector2i() : this(0, 0) { }

        public Vector2i(int xy) : this(xy, xy) { }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (obj.GetType() != typeof(Vector2i)) return false;
            
            Vector2i other = (Vector2i) obj;
            return CoreC.Equals(X, other.X) && CoreC.Equals(Y, other.Y);
        }

        public override int GetHashCode() => HashCode.Combine(X, Y);
    }
}
