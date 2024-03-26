namespace Xnor.Core
{
    public struct Vector3(float x, float y, float z)
    {
        public static readonly Vector3 Zero = new();
        
        public static readonly Vector3 UnitX = new(1f, 0f, 0f);
        
        public static readonly Vector3 UnitY = new(0f, 1f, 0f);
        
        public static readonly Vector3 UnitZ = new(0f, 0f, 1f);
        
        public static readonly Vector3 One = new(1f);

        public static Vector3 operator+(Vector3 a, Vector3 b) => new(a.X + b.X, a.Y + b.Y, a.Z + b.Z);

        public static Vector3 operator-(Vector3 m) => new(-m.X, -m.Y, -m.Z);

        public static Vector3 operator-(Vector3 a, Vector3 b) => a + -b;

        public static Vector3 operator*(Vector3 a, Vector3 b) => new(a.X * b.X, a.Y * b.Y, a.Z * b.Z);

        public static Vector3 operator*(Vector3 a, float scalar) => new(a.X * scalar, a.Y * scalar, a.Z * scalar);

        public static Vector3 operator*(float scalar, Vector3 b) => b * scalar;

        public static Vector3 operator/(Vector3 a, Vector3 b) => new(a.X / b.X, a.Y / b.Y, a.Z / b.Z);

        public static Vector3 operator/(Vector3 a, float scalar) => new(a.X / scalar, a.Y / scalar, a.Z / scalar);

        public static bool operator==(Vector3 a, Vector3 b) => a.Equals(b);

        public static bool operator!=(Vector3 a, Vector3 b) => !(a == b);

        public static explicit operator Vector2(Vector3 v) => new(v.X, v.Y);

        public static explicit operator Vector4(Vector3 v) => new(v.X, v.Y, 0f, 1f);
        
        public float X = x;
        public float Y = y;
        public float Z = z;

        public Vector3() : this(0f, 0f, 0f) { }

        public Vector3(float xyz) : this(xyz, xyz, xyz) { }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (obj.GetType() != typeof(Vector3)) return false;
            
            Vector3 other = (Vector3) obj;
            return CoreC.Equals(X, other.X) && CoreC.Equals(Y, other.Y);
        }

        public override int GetHashCode() => HashCode.Combine(X, Y);
    }
}
