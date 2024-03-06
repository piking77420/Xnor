#include "world/scene_graph.hpp"

#include <Maths/matrix.hpp>

using namespace XnorCore;

Vector3 ScaleVector(const Vector3& v,const float desiredLength)
{
	return v * desiredLength / v.Length();
}

Vector3 Combine(const Vector3& a, const Vector3& b, const float_t ascl, const float_t bscl)
{
	return (a * ascl) + (b * bscl);
}

bool_t Decompose(const Matrix& modelMatrix, Vector3* const translation,
	Quaternion* const orientation, Vector3* const scale,
	Vector3* const skew, Vector4* const perspective)
{
	Matrix localMatrix(modelMatrix);

	if (Calc::IsZero(localMatrix.m33))
		return false;
	
	// Normalize the matrix.
	float_t invm33 = 1.f / localMatrix[3][3];
	float_t* matrix = localMatrix.Raw();
	
	for (uint32_t i = 0; i < 16; i++)
	{
		matrix[i] *= invm33;
	}

	Matrix perspectiveMatrix(localMatrix);
	perspectiveMatrix.m30 = 0.f;
	perspectiveMatrix.m31 = 0.f;
	perspectiveMatrix.m32 = 0.f;
	perspectiveMatrix.m33 = 1.f;

	if (Calc::IsZero(perspectiveMatrix.Determinant(), 0.f))
		return false;

	// First, isolate perspective.  This is the messiest.
	if(Calc::IsZero(localMatrix.m30,0.f) ||Calc::IsZero(localMatrix.m31,0.f) ||
		Calc::IsZero(localMatrix.m32,0.f) )
	{
		// rightHandSide is the right hand side of the equation.
		Vector4 rightHandSide;
		rightHandSide[0] = localMatrix[0][3];
		rightHandSide[1] = localMatrix[1][3];
		rightHandSide[2] = localMatrix[2][3];
		rightHandSide[3] = localMatrix[3][3];
		
		Matrix inversePerspectiveMatrix = perspectiveMatrix.Inverted();
		Matrix transposedInversePerspectiveMatrix = inversePerspectiveMatrix.Transposed();

		*perspective = transposedInversePerspectiveMatrix * rightHandSide;

		// Clear the perspective partition
		localMatrix.m30 = localMatrix.m13 = localMatrix.m23 = static_cast<float_t>(0);
		localMatrix.m33 = static_cast<float_t>(1);
	}
	else
	{
		*perspective = Vector4(0.f, 0.f, 0.f, 1.f);
	}

	// Next take care of translation (easy).
	*translation = Vector3(localMatrix[3]);
	Vector4& localMatrixColoms4 = reinterpret_cast<Vector4&>(localMatrix.m03); 
	localMatrixColoms4 = Vector4(0, 0, 0, localMatrix[3].w);

	Vector3 row[3], pdum3;

	// Now get scale and shear.
	for (int32_t i = 0; i < 3; ++i)
	{
		for (int32_t j = 0; j < 3; ++j)
			row[i][j] = localMatrix[static_cast<uint8_t>(i)][j];
	}

	// Compute X scale factor and normalize first row.
	scale->x = row[0].Length();

	row[0] = ScaleVector(row[0],1.f);

	// Compute XY shear factor and make 2nd row orthogonal to 1st.
	skew->z = Vector3::Dot(row[0], row[1]);
	row[1] = Combine(row[1], row[0], 1.f, -skew->z);

	// Now, compute Y scale and normalize 2nd row.
	scale->y = row[1].Length();
	row[1] = ScaleVector(row[1], 1.f);
	skew->z /= scale->y;

	// Compute XZ and YZ shears, orthogonalize 3rd row.
	skew->y = Vector3::Dot(row[0], row[2]);
	row[2] = Combine(row[2], row[0], 1.f, -skew->y);
	skew->x = Vector3::Dot(row[1], row[2]);
	row[2] = Combine(row[2], row[1], 1.f, -skew->x);

	// Next, get Z scale and normalize 3rd row.
	scale->z = row[2].Length();
	row[2] = ScaleVector(row[2], 1.f);
	skew->y /= scale->z;
	skew->x /= scale->z;

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	pdum3 = Vector3::Cross(row[1], row[2]); 
	if (Vector3::Dot(row[0], pdum3) < 0)
	{
		for(uint32_t i = 0; i < 3; i++)
		{
			scale[i] *= -1.f;
			row[i] *= -1.f;
		}
	}

	int32_t i, j, k = 0;
	float_t root = 0.f;
	float_t trace = row[0].x + row[1].y + row[2].z;
	
	if (trace > 0.f)
	{
		root = sqrt(trace + 1.0f);
		orientation->W() = 0.5f * root;
		root = 0.5f / root;
		orientation->X() = root * (row[1].z - row[2].y);
		orientation->Y() = root * (row[2].x - row[0].z);
		orientation->Z() = root * (row[0].y - row[1].x);
	} 
	else
	{
		static int32_t next[3] = {1, 2, 0};
		i = 0;
		if (row[1].y > row[0].x)
			i = 1;
		if (row[2].z > row[i][i])
			i = 2;
		j = next[i];
		k = next[j];

		root = sqrt(row[i][i] - row[j][j] - row[k][k] + 1.0f);

		// ref operator [] ambiguous with Quaternion*
		Quaternion& ref = *orientation;
		ref[i] = 0.5f * root;
		root = 0.5f / root;
		ref[j] = root * (row[i][j] + row[j][i]);
		ref[k] = root * (row[i][k] + row[k][i]);
		ref.W() = root * (row[j][k] - row[k][j]);
	} 

	return true;
}


Matrix GetTrsOfParents(const Entity& parent)
{
	const Matrix parentMatrix = Matrix::Trs(parent.transform.position, parent.transform.rotation, parent.transform.scale);

	if (parent.HasParent())  
	{
		return GetTrsOfParents(*parent.GetParent()) * parentMatrix;
	}
	
	return parentMatrix;
}

void UpdateTransform(Entity& entity)
{
	Transform& t = entity.transform;

	if (t.changed)
	{
		t.changed = false;

		t.rotation = Quaternion::FromEuler(t.eulerRotation).Normalized();
		t.worldMatrix = Matrix::Trs(t.position, t.rotation, t.scale);
	    
		if (!entity.HasParent())
			return;
		
		t.worldMatrix = GetTrsOfParents(*entity.GetParent()) *  t.worldMatrix;
	}
}


void SceneGraph::Update(const List<Entity*>& entities)
{
    for (size_t i = 0; i < entities.GetSize(); i++)
    {
        UpdateTransform(*entities[i]);
    }
}

void SceneGraph::OnAttachToParent(Entity& entity)
{
    Transform& transform = entity.transform;

    Matrix trs = transform.worldMatrix;
    const Matrix parent = GetTrsOfParents(*entity.GetParent());
    
    trs = parent.Inverted() * trs;
	Vector3 skew;
	Vector4 perspective;
	
    Decompose(trs, &transform.position, &transform.rotation, &transform.scale, &skew, &perspective);
	transform.position = static_cast<Vector3>(trs[3]);
	transform.eulerRotation = Quaternion::ToEuler(transform.rotation);
}
