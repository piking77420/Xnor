namespace Xnor.Core
{
    public struct Vector4(float x, float y, float z, float w)
    {
        public static readonly Vector4 Zero = new();
        
        public static readonly Vector4 UnitX = new(1f, 0f, 0f, 0f);
        
        public static readonly Vector4 UnitY = new(0f, 1f, 0f, 0f);
        
        public static readonly Vector4 UnitZ = new(0f, 0f, 1f, 0f);
        
        public static readonly Vector4 UnitW = new(0f, 0f, 0f, 1f);
        
        public static readonly Vector4 One = new(1f);

        public static Vector4 operator+(Vector4 a, Vector4 b) => new(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);

        public static Vector4 operator-(Vector4 m) => new(-m.X, -m.Y, -m.Z, -m.W);

        public static Vector4 operator-(Vector4 a, Vector4 b) => a + -b;

        public static Vector4 operator*(Vector4 a, Vector4 b) => new(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W);

        public static Vector4 operator*(Vector4 a, float scalar) => new(a.X * scalar, a.Y * scalar, a.Z * scalar, a.W * scalar);

        public static Vector4 operator*(float scalar, Vector4 b) => b * scalar;

        public static Vector4 operator/(Vector4 a, Vector4 b) => new(a.X / b.X, a.Y / b.Y, a.Z / b.Z, a.W / b.W);

        public static Vector4 operator/(Vector4 a, float scalar) => new(a.X / scalar, a.Y / scalar, a.Z / scalar, a.W / scalar);

        public static bool operator==(Vector4 a, Vector4 b) => a.Equals(b);

        public static bool operator!=(Vector4 a, Vector4 b) => !(a == b);

        public static explicit operator Vector2(Vector4 v) => new(v.X, v.Y);

        public static explicit operator Vector3(Vector4 v) => new(v.X, v.Y, v.Z);
        
        public float X = x;
        public float Y = y;
        public float Z = z;
        public float W = w;

        public Vector4() : this(0f, 0f, 0f, 0f) { }

        public Vector4(float xyzw) : this(xyzw, xyzw, xyzw, xyzw) { }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (obj.GetType() != typeof(Vector4)) return false;
            
            Vector4 other = (Vector4) obj;
            return CoreC.Equals(X, other.X) && CoreC.Equals(Y, other.Y);
        }

        public override int GetHashCode() => HashCode.Combine(X, Y);
    }
}
