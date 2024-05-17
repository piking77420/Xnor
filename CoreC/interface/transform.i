%module CoreC

%typemap(cscode) XnorCore::Transform
%{
    public Vector3 Position { get => GetPosition(); set => SetPosition(value); }

    public float PositionX { get => Position.X; set => SetPositionX(value); }

    public float PositionY { get => Position.Y; set => SetPositionY(value); }

    public float PositionZ { get => Position.Z; set => SetPositionZ(value); }
    
    public Quaternion Rotation { get => GetRotation(); set => SetRotation(value); }

    public float RotationX { get => Rotation.X; set => SetRotationX(value); }

    public float RotationY { get => Rotation.Y; set => SetRotationY(value); }

    public float RotationZ { get => Rotation.Z; set => SetRotationZ(value); }

    public float RotationW { get => Rotation.W; set => SetRotationW(value); }
    
    public Vector3 RotationEulerAngle { get => GetRotationEulerAngle(); set => SetRotationEulerAngle(value); }

    public float RotationEulerAngleX { get => RotationEulerAngle.X; set => SetRotationEulerAngleX(value); }

    public float RotationEulerAngleY { get => RotationEulerAngle.Y; set => SetRotationEulerAngleY(value); }

    public float RotationEulerAngleZ { get => RotationEulerAngle.Z; set => SetRotationEulerAngleZ(value); }
    
    public Vector3 Scale { get => GetScale(); set => SetScale(value); }

    public float ScaleX { get => Scale.X; set => SetScaleX(value); }

    public float ScaleY { get => Scale.Y; set => SetScaleY(value); }

    public float ScaleZ { get => Scale.Z; set => SetScaleZ(value); }
%}

%include "transform.hpp"
