%module Core

%{
#include "Maths/math.hpp"
%}

%include "Maths/definitions.hpp"

%include "Maths/calc.hpp"

%include "Maths/easing.hpp"

%ignore Matrix::Raw;
%ignore Matrix::Matrix(float*);

%csmethodmodifiers Matrix::operator Matrix3 "private";

%rename(ToMatrix3) Matrix::operator Matrix3;

%typemap(cscode) Matrix
%{
    public static Matrix operator+(Matrix a, Matrix b) { return Core.OperatorAdd(a, b); }

    public static Matrix operator-(Matrix m) { return Core.OperatorSub(m); }

    public static Matrix operator-(Matrix a, Matrix b) { return Core.OperatorSub(a, b); }

    public static Matrix operator*(Matrix a, float b) { return Core.OperatorMul(a, b); }

    public static Matrix operator*(float a, Matrix b) { return Core.OperatorMul(b, a); }

    public static Vector3 operator*(Matrix a, Vector3 b) { return Core.OperatorMul(a, b); }

    public static Vector4 operator*(Matrix a, Vector4 b) { return Core.OperatorMul(a, b); }

    public static Matrix operator*(Matrix a, Matrix b) { return Core.OperatorMul(a, b); }

    public static bool operator==(Matrix a, Matrix b) { return a.Equals(b); }

    public static bool operator!=(Matrix a, Matrix b) { return !a.Equals(b); }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Core.OperatorEq(this, (Matrix) obj);
    }

    public override int GetHashCode()
    {
        HashCode hash = new();

        hash.Add(m00);
        hash.Add(m01);
        hash.Add(m02);
        hash.Add(m03);

        hash.Add(m10);
        hash.Add(m11);
        hash.Add(m12);
        hash.Add(m13);

        hash.Add(m20);
        hash.Add(m21);
        hash.Add(m22);
        hash.Add(m23);

        hash.Add(m30);
        hash.Add(m31);
        hash.Add(m32);
        hash.Add(m33);

        return hash.ToHashCode();
    }

    public static explicit operator Matrix3(Matrix m) { return m.ToMatrix3(); }
%}

%include "Maths/matrix.hpp"

%ignore Matrix3::Raw;
%ignore Matrix3::Matrix3(float*);

%csmethodmodifiers Matrix3::operator Matrix "private";

%rename(ToMatrix) Matrix3::operator Matrix;

%typemap(cscode) Matrix3
%{
    public static Matrix3 operator+(Matrix3 a, Matrix3 b) { return Core.OperatorAdd(a, b); }

    public static Matrix3 operator-(Matrix3 m) { return Core.OperatorSub(m); }

    public static Matrix3 operator-(Matrix3 a, Matrix3 b) { return Core.OperatorSub(a, b); }

    public static Matrix3 operator*(Matrix3 a, float b) { return Core.OperatorMul(a, b); }

    public static Matrix3 operator*(float a, Matrix3 b) { return Core.OperatorMul(b, a); }

    public static Vector3 operator*(Matrix3 a, Vector3 b) { return Core.OperatorMul(a, b); }

    public static Matrix3 operator*(Matrix3 a, Matrix3 b) { return Core.OperatorMul(a, b); }

    public static bool operator==(Matrix3 a, Matrix3 b) { return a.Equals(b); }

    public static bool operator!=(Matrix3 a, Matrix3 b) { return !a.Equals(b); }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Core.OperatorEq(this, (Matrix3) obj);
    }

    public override int GetHashCode()
    {
        HashCode hash = new();

        hash.Add(m00);
        hash.Add(m01);
        hash.Add(m02);

        hash.Add(m10);
        hash.Add(m11);
        hash.Add(m12);

        hash.Add(m20);
        hash.Add(m21);
        hash.Add(m22);

        return hash.ToHashCode();
    }

    public static explicit operator Matrix(Matrix3 m) { return m.ToMatrix(); }
%}

%include "Maths/matrix3.hpp"

%ignore Vector2::Raw;
%ignore Vector2::Vector2(float*);

%csmethodmodifiers Vector2::operator Vector2i "private";
%csmethodmodifiers Vector2::operator Vector3 "private";
%csmethodmodifiers Vector2::operator Vector4 "private";

%rename(ToVector2i) Vector2::operator Vector2i;
%rename(ToVector3) Vector2::operator Vector3;
%rename(ToVector4) Vector2::operator Vector4;

%typemap(cscode) Vector2
%{
    public static Vector2 operator+(Vector2 a, Vector2 b) { return Core.OperatorAdd(a, b); }

    public static Vector2 operator-(Vector2 m) { return Core.OperatorSub(m); }

    public static Vector2 operator-(Vector2 a, Vector2 b) { return Core.OperatorSub(a, b); }

    public static Vector2 operator*(Vector2 a, Vector2 b) { return Core.OperatorMul(a, b); }

    public static Vector2 operator*(Vector2 a, float b) { return Core.OperatorMul(a, b); }

    public static Vector2 operator*(float a, Vector2 b) { return Core.OperatorMul(b, a); }

    public static Vector2 operator/(Vector2 a, Vector2 b) { return Core.OperatorDiv(a, b); }

    public static Vector2 operator/(Vector2 a, float b) { return Core.OperatorDiv(a, b); }

    public static bool operator==(Vector2 a, Vector2 b) { return a.Equals(b); }

    public static bool operator!=(Vector2 a, Vector2 b) { return !a.Equals(b); }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Core.OperatorEq(this, (Vector2) obj);
    }

    public override int GetHashCode() { return HashCode.Combine(x, y); }

    public static explicit operator Vector2i(Vector2 v) { return v.ToVector2i(); }

    public static explicit operator Vector3(Vector2 v) { return v.ToVector3(); }

    public static explicit operator Vector4(Vector2 v) { return v.ToVector4(); }
%}

%include "Maths/vector2.hpp"

%ignore Vector2i::Raw;
%ignore Vector2i::Vector2i(float*);

%csmethodmodifiers Vector2i::operator Vector2 "private";
%csmethodmodifiers Vector2i::operator Vector3 "private";
%csmethodmodifiers Vector2i::operator Vector4 "private";

%rename(ToVector2) Vector2i::operator Vector2;
%rename(ToVector3) Vector2i::operator Vector3;
%rename(ToVector4) Vector2i::operator Vector4;

%typemap(cscode) Vector2i
%{
    public static Vector2i operator+(Vector2i a, Vector2i b) { return Core.OperatorAdd(a, b); }

    public static Vector2i operator-(Vector2i m) { return Core.OperatorSub(m); }

    public static Vector2i operator-(Vector2i a, Vector2i b) { return Core.OperatorSub(a, b); }

    public static Vector2i operator*(Vector2i a, int b) { return Core.OperatorMul(a, b); }

    public static Vector2i operator*(int a, Vector2i b) { return Core.OperatorMul(b, a); }

    public static Vector2i operator*(Vector2i a, Vector2i b) { return Core.OperatorMul(a, b); }

    public static Vector2 operator/(Vector2i a, Vector2i b) { return Core.OperatorDiv(a, b); }

    public static Vector2 operator/(Vector2i a, float b) { return Core.OperatorDiv(a, b); }

    public static bool operator==(Vector2i a, Vector2i b) { return a.Equals(b); }

    public static bool operator!=(Vector2i a, Vector2i b) { return !a.Equals(b); }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Core.OperatorEq(this, (Vector2i) obj);
    }

    public override int GetHashCode() { return HashCode.Combine(x, y); }

    public static explicit operator Vector2(Vector2i v) { return v.ToVector2(); }

    public static explicit operator Vector3(Vector2i v) { return v.ToVector3(); }

    public static explicit operator Vector4(Vector2i v) { return v.ToVector4(); }
%}

%include "Maths/vector2i.hpp"

%ignore Vector3::Raw;
%ignore Vector3::Vector3(float*);

%csmethodmodifiers Vector3::operator Vector2 "private";
%csmethodmodifiers Vector3::operator Vector4 "private";

%rename(ToVector2) Vector3::operator Vector2;
%rename(ToVector4) Vector3::operator Vector4;

%typemap(cscode) Vector3
%{
    public static Vector3 operator+(Vector3 a, Vector3 b) { return Core.OperatorAdd(a, b); }

    public static Vector3 operator-(Vector3 m) { return Core.OperatorSub(m); }

    public static Vector3 operator-(Vector3 a, Vector3 b) { return Core.OperatorSub(a, b); }

    public static Vector3 operator*(Vector3 a, float b) { return Core.OperatorMul(a, b); }

    public static Vector3 operator*(float a, Vector3 b) { return Core.OperatorMul(b, a); }

    public static Vector3 operator*(Vector3 a, Vector3 b) { return Core.OperatorMul(a, b); }

    public static Vector3 operator/(Vector3 a, Vector3 b) { return Core.OperatorDiv(a, b); }

    public static Vector3 operator/(Vector3 a, float b) { return Core.OperatorDiv(a, b); }

    public static bool operator==(Vector3 a, Vector3 b) { return a.Equals(b); }

    public static bool operator!=(Vector3 a, Vector3 b) { return !a.Equals(b); }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Core.OperatorEq(this, (Vector3) obj);
    }

    public override int GetHashCode() { return HashCode.Combine(x, y, z); }

    public static explicit operator Vector2(Vector3 v) { return v.ToVector2(); }

    public static explicit operator Vector4(Vector3 v) { return v.ToVector4(); }
%}

%include "Maths/vector3.hpp"

%ignore Vector4::Raw;
%ignore Vector4::Vector4(float*);

%csmethodmodifiers Vector4::operator Vector2 "private";
%csmethodmodifiers Vector4::operator Vector3 "private";
%csmethodmodifiers Vector4::operator Matrix "private";

%rename(ToVector2) Vector4::operator Vector2;
%rename(ToVector3) Vector4::operator Vector3;
%rename(ToMatrix) Vector4::operator Matrix;

%typemap(cscode) Vector4
%{
    public static Vector4 operator+(Vector4 a, Vector4 b) { return Core.OperatorAdd(a, b); }

    public static Vector4 operator-(Vector4 m) { return Core.OperatorSub(m); }

    public static Vector4 operator-(Vector4 a, Vector4 b) { return Core.OperatorSub(a, b); }

    public static Vector4 operator*(Vector4 a, float b) { return Core.OperatorMul(a, b); }

    public static Vector4 operator*(float a, Vector4 b) { return Core.OperatorMul(b, a); }

    public static Vector4 operator*(Vector4 a, Vector4 b) { return Core.OperatorMul(a, b); }

    public static Vector4 operator/(Vector4 a, Vector4 b) { return Core.OperatorDiv(a, b); }

    public static Vector4 operator/(Vector4 a, float b) { return Core.OperatorDiv(a, b); }

    public static bool operator==(Vector4 a, Vector4 b) { return a.Equals(b); }

    public static bool operator!=(Vector4 a, Vector4 b) { return !a.Equals(b); }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Core.OperatorEq(this, (Vector4) obj);
    }

    public override int GetHashCode() { return HashCode.Combine(x, y, z, w); }

    public static explicit operator Vector2(Vector4 v) { return v.ToVector2(); }

    public static explicit operator Vector3(Vector4 v) { return v.ToVector3(); }

    public static explicit operator Matrix(Vector4 v) { return v.ToMatrix(); }
%}

%include "Maths/vector4.hpp"

%ignore Quaternion::Raw;
%ignore Quaternion::Quaternion(float*);

%csmethodmodifiers Quaternion::operator Vector3 "private";
%csmethodmodifiers Quaternion::operator Vector4 "private";

%rename(ToVector3) Quaternion::operator Vector3;
%rename(ToVector4) Quaternion::operator Vector4;

%typemap(cscode) Quaternion
%{
    public static Quaternion operator+(Quaternion a, Quaternion b) { return Core.OperatorAdd(a, b); }

    public static Quaternion operator-(Quaternion m) { return Core.OperatorSub(m); }

    public static Quaternion operator-(Quaternion a, Quaternion b) { return Core.OperatorSub(a, b); }

    public static Quaternion operator*(Quaternion a, float b) { return Core.OperatorMul(a, b); }

    public static Quaternion operator*(float a, Quaternion b) { return Core.OperatorMul(b, a); }

    public static Quaternion operator*(Quaternion a, Quaternion b) { return Core.OperatorMul(a, b); }

    public static Quaternion operator/(Quaternion a, float b) { return Core.OperatorDiv(a, b); }

    public static bool operator==(Quaternion a, Quaternion b) { return a.Equals(b); }

    public static bool operator!=(Quaternion a, Quaternion b) { return !a.Equals(b); }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Core.OperatorEq(this, (Quaternion) obj);
    }

    public override int GetHashCode() { return HashCode.Combine(X(), Y(), Z(), W()); }

    public static explicit operator Vector3(Quaternion q) { return q.ToVector3(); }

    public static explicit operator Vector4(Quaternion q) { return q.ToVector4(); }
%}

%include "Maths/quaternion.hpp"
