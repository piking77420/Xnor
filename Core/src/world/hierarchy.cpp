#include "world/hierarchy.hpp"

#include <Maths/matrix.hpp>

using namespace XnorCore;

Vector3 ScaleVector(const Vector3& v, float desiredLength)
{
	return v * desiredLength / v.Length();
}

Vector3 combine(
	Vector3 const& a,
	Vector3 const& b,
	float ascl, float bscl)
{
	return (a * ascl) + (b * bscl);
}

bool decompose(Matrix const& ModelMatrix, Vector3& Translation, Quaternion & Orientation,Vector3 & Scale,
	 Vector3& Skew, Vector4& Perspective)
{
	Matrix LocalMatrix(ModelMatrix);

	// Normalize the matrix.
	if(Calc::IsZero(LocalMatrix.m33))
		return false;

	float_t Invm33 = 1.f/LocalMatrix[3][3];
	float_t* matrix = LocalMatrix.Raw();
	
	for (uint32_t i = 0; i < 16; i++)
	{
		matrix[i] *= Invm33;
	}

	// perspectiveMatrix is used to solve for perspective, but it also provides
	// an easy way to test for singularity of the upper 3x3 component.
	Matrix PerspectiveMatrix(LocalMatrix);
	PerspectiveMatrix.m30 = 0.f;
	PerspectiveMatrix.m31 = 0.f;
	PerspectiveMatrix.m32 = 0.f;
	PerspectiveMatrix.m33 = 1.f;

	/// TODO: Fixme!
	if(Calc::IsZero(PerspectiveMatrix.Determinant(), 0.f))
		return false;

	// First, isolate perspective.  This is the messiest.
	if(
		Calc::IsZero(LocalMatrix.m30,0.f) ||
		Calc::IsZero(LocalMatrix.m31,0.f) ||
		Calc::IsZero(LocalMatrix.m32,0.f) )
	{
		// rightHandSide is the right hand side of the equation.
		Vector4 RightHandSide;
		RightHandSide[0] = LocalMatrix[0][3];
		RightHandSide[1] = LocalMatrix[1][3];
		RightHandSide[2] = LocalMatrix[2][3];
		RightHandSide[3] = LocalMatrix[3][3];

		// Solve the equation by inverting PerspectiveMatrix and multiplying
		// rightHandSide by the inverse.  (This is the easiest way, not
		// necessarily the best.)
		Matrix InversePerspectiveMatrix = PerspectiveMatrix.Inverted();//   inverse(PerspectiveMatrix, inversePerspectiveMatrix);
		Matrix TransposedInversePerspectiveMatrix = InversePerspectiveMatrix.Transposed();//   transposeMatrix4(inversePerspectiveMatrix, transposedInversePerspectiveMatrix);

		Perspective = TransposedInversePerspectiveMatrix * RightHandSide;
		//  v4MulPointByMatrix(rightHandSide, transposedInversePerspectiveMatrix, perspectivePoint);

		// Clear the perspective partition
		LocalMatrix.m30 = LocalMatrix.m13 = LocalMatrix.m23 = static_cast<float_t>(0);
		LocalMatrix.m33 = static_cast<float_t>(1);
	}
	else
	{
		// No perspective.
		Perspective = Vector4(0.f, 0.f, 0.f, 1.f);
	}

	// Next take care of translation (easy).
	Translation = Vector3(LocalMatrix[3]);
	Vector4& localMatrixColoms4 = reinterpret_cast<Vector4&>(LocalMatrix.m03); 
	localMatrixColoms4 = Vector4(0, 0, 0, LocalMatrix[3].w);

	Vector3 Row[3], Pdum3;

	// Now get scale and shear.
	for(uint32_t i = 0; i < 3; ++i)
	for(uint32_t j = 0; j < 3; ++j)
		Row[i][j] = LocalMatrix[i][j];

	// Compute X scale factor and normalize first row.
	Scale.x = Row[0].Length();// v3Length(Row[0]);

	Row[0] = ScaleVector(Row[0], static_cast<float_t>(1));

	// Compute XY shear factor and make 2nd row orthogonal to 1st.
	Skew.z = Vector3::Dot(Row[0], Row[1]);
	Row[1] = combine(Row[1], Row[0], static_cast<float_t>(1), -Skew.z);

	// Now, compute Y scale and normalize 2nd row.
	Scale.y = Row[1].Length();
	Row[1] = ScaleVector(Row[1], static_cast<float_t>(1));
	Skew.z /= Scale.y;

	// Compute XZ and YZ shears, orthogonalize 3rd row.
	Skew.y = Vector3::Dot(Row[0], Row[2]);
	Row[2] = combine(Row[2], Row[0], static_cast<float_t>(1), -Skew.y);
	Skew.x = Vector3::Dot(Row[1], Row[2]);
	Row[2] = combine(Row[2], Row[1], static_cast<float_t>(1), -Skew.x);

	// Next, get Z scale and normalize 3rd row.
	Scale.z = Row[2].Length();
	Row[2] = ScaleVector(Row[2], static_cast<float_t>(1));
	Skew.y /= Scale.z;
	Skew.x /= Scale.z;

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	Pdum3 = Vector3::Cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
	if(Vector3::Dot(Row[0], Pdum3) < 0)
	{
		for(uint32_t i = 0; i < 3; i++)
		{
			Scale[i] *= -1.f;
			Row[i] *= -1.f;
		}
	}

	// Now, get the rotations out, as described in the gem.

	// FIXME - Add the ability to return either quaternions (which are
	// easier to recompose with) or Euler angles (rx, ry, rz), which
	// are easier for authors to deal with. The latter will only be useful
	// when we fix https://bugs.webkit.org/show_bug.cgi?id=23799, so I
	// will leave the Euler angle code here for now.

	// ret.rotateY = asin(-Row[0][2]);
	// if (cos(ret.rotateY) != 0) {
	//     ret.rotateX = atan2(Row[1][2], Row[2][2]);
	//     ret.rotateZ = atan2(Row[0][1], Row[0][0]);
	// } else {
	//     ret.rotateX = atan2(-Row[2][0], Row[1][1]);
	//     ret.rotateZ = 0;
	// }

	int i, j, k = 0;
	float_t root, trace = Row[0].x + Row[1].y + Row[2].z;
	if(trace > static_cast<float_t>(0))
	{
		root = sqrt(trace + static_cast<float_t>(1.0));
		Orientation.W() = static_cast<float_t>(0.5) * root;
		root = static_cast<float_t>(0.5) / root;
		Orientation.X() = root * (Row[1].z - Row[2].y);
		Orientation.Y() = root * (Row[2].x - Row[0].z);
		Orientation.Z() = root * (Row[0].y - Row[1].x);
	} // End if > 0
	else
	{
		static int Next[3] = {1, 2, 0};
		i = 0;
		if(Row[1].y > Row[0].x) i = 1;
		if(Row[2].z > Row[i][i]) i = 2;
		j = Next[i];
		k = Next[j];

		root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<float_t>(1.0f));

		Orientation[i] = static_cast<float_t>(0.5) * root;
		root = static_cast<float_t>(0.5) / root;
		Orientation[j] = root * (Row[i][j] + Row[j][i]);
		Orientation[k] = root * (Row[i][k] + Row[k][i]);
		Orientation.W() = root * (Row[j][k] - Row[k][j]);
	} // End if <= 0

	return true;
}



void Hierarchy::Update(const List<Entity*>& entities)
{
    for (uint32_t i = 0; i < entities.GetSize(); i++)
    {
        UpdateTransform(*entities[i]);
    }
}

Matrix Hierarchy::GetTrs(const Matrix& trs,const Entity* entity)
{
    if(!entity->HasParent())
        return trs;
    
    return GetTrsOfParents(*entity->GetParent()) * trs;
}

void Hierarchy::OnAttachToParent(Entity& entity)
{
    Transform& transform = entity.transform;

    Matrix&& trs = Matrix::Trs(transform.position,transform.quaternion,transform.scale);
    Matrix&& parent = GetTrsOfParents(*entity.GetParent());
    
    trs = parent.Inverted() * trs ;
	Vector3 skew;
	Vector4 perspective;

    decompose(trs,transform.position,transform.quaternion,transform.scale,skew,perspective);
	transform.position = static_cast<Vector3>(trs[3]);
	transform.rotation = Quaternion::ToEuler(transform.quaternion);
}

Matrix Hierarchy::GetTrsOfParents(const Entity& parent)
{
    Matrix&& parentMatrix = Matrix::Trs(parent.transform.position,parent.transform.quaternion,parent.transform.scale);

    if(parent.HasParent())  
    {
        return GetTrsOfParents(*parent.GetParent()) * parentMatrix;
    }
    return parentMatrix;
}

void Hierarchy::UpdateTransform(Entity& entity)
{
    entity.transform.quaternion = Quaternion::FromEuler(entity.transform.rotation).Normalized();
    
    if(!entity.HasParent())
        return;
        
    Matrix&& trs = Matrix::Trs(entity.transform.position,entity.transform.quaternion,entity.transform.scale);
    
    trs = GetTrsOfParents(*entity.GetParent()) * trs;
}
