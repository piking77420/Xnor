#include "world/scene_graph.hpp"

#include <Maths/matrix.hpp>

using namespace XnorCore;

Vector3 ScaleVector(const Vector3& v,const float desiredLength)
{
	return v * desiredLength / v.Length();
}

Vector3 Combine(Vector3 const& a,Vector3 const& b, const float ascl,const float bscl)
{
	return (a * ascl) + (b * bscl);
}

bool Decompose(Matrix const& modelMatrix, Vector3* translation,
	Quaternion* orientation,Vector3* Scale,
	Vector3* skew, Vector4* perspective)
{
	Matrix LocalMatrix(modelMatrix);

	if (Calc::IsZero(LocalMatrix.m33))
		return false;
	
	// Normalize the matrix.
	float_t Invm33 = 1.f/LocalMatrix[3][3];
	float_t* matrix = LocalMatrix.Raw();
	
	for (uint32_t i = 0; i < 16; i++)
	{
		matrix[i] *= Invm33;
	}

	Matrix PerspectiveMatrix(LocalMatrix);
	PerspectiveMatrix.m30 = 0.f;
	PerspectiveMatrix.m31 = 0.f;
	PerspectiveMatrix.m32 = 0.f;
	PerspectiveMatrix.m33 = 1.f;

	if(Calc::IsZero(PerspectiveMatrix.Determinant(), 0.f))
		return false;

	// First, isolate perspective.  This is the messiest.
	if(Calc::IsZero(LocalMatrix.m30,0.f) ||Calc::IsZero(LocalMatrix.m31,0.f) ||
		Calc::IsZero(LocalMatrix.m32,0.f) )
	{
		// rightHandSide is the right hand side of the equation.
		Vector4 RightHandSide;
		RightHandSide[0] = LocalMatrix[0][3];
		RightHandSide[1] = LocalMatrix[1][3];
		RightHandSide[2] = LocalMatrix[2][3];
		RightHandSide[3] = LocalMatrix[3][3];
		
		Matrix InversePerspectiveMatrix = PerspectiveMatrix.Inverted();
		Matrix TransposedInversePerspectiveMatrix = InversePerspectiveMatrix.Transposed();

		*perspective = TransposedInversePerspectiveMatrix * RightHandSide;

		// Clear the perspective partition
		LocalMatrix.m30 = LocalMatrix.m13 = LocalMatrix.m23 = static_cast<float_t>(0);
		LocalMatrix.m33 = static_cast<float_t>(1);
	}
	else
	{
		*perspective = Vector4(0.f, 0.f, 0.f, 1.f);
	}

	// Next take care of translation (easy).
	*translation = Vector3(LocalMatrix[3]);
	Vector4& localMatrixColoms4 = reinterpret_cast<Vector4&>(LocalMatrix.m03); 
	localMatrixColoms4 = Vector4(0, 0, 0, LocalMatrix[3].w);

	Vector3 Row[3], Pdum3;

	// Now get scale and shear.
	for(int32_t i = 0; i < 3; ++i)
	for(int32_t j = 0; j < 3; ++j)
		Row[i][j] = LocalMatrix[static_cast<uint8_t>(i)][j];

	// Compute X scale factor and normalize first row.
	Scale->x = Row[0].Length();

	Row[0] = ScaleVector(Row[0],1.f);

	// Compute XY shear factor and make 2nd row orthogonal to 1st.
	skew->z = Vector3::Dot(Row[0], Row[1]);
	Row[1] = Combine(Row[1], Row[0], 1.f, -skew->z);

	// Now, compute Y scale and normalize 2nd row.
	Scale->y = Row[1].Length();
	Row[1] = ScaleVector(Row[1], 1.f);
	skew->z /= Scale->y;

	// Compute XZ and YZ shears, orthogonalize 3rd row.
	skew->y = Vector3::Dot(Row[0], Row[2]);
	Row[2] = Combine(Row[2], Row[0], 1.f, -skew->y);
	skew->x = Vector3::Dot(Row[1], Row[2]);
	Row[2] = Combine(Row[2], Row[1], 1.f, -skew->x);

	// Next, get Z scale and normalize 3rd row.
	Scale->z = Row[2].Length();
	Row[2] = ScaleVector(Row[2], 1.f);
	skew->y /= Scale->z;
	skew->x /= Scale->z;

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
	Pdum3 = Vector3::Cross(Row[1], Row[2]); 
	if(Vector3::Dot(Row[0], Pdum3) < 0)
	{
		for(uint32_t i = 0; i < 3; i++)
		{
			Scale[i] *= -1.f;
			Row[i] *= -1.f;
		}
	}

	int32_t i, j, k = 0;
	float_t root = 0.f;
	float_t trace = Row[0].x + Row[1].y + Row[2].z;
	
	if (trace > 0.f)
	{
		root = sqrt(trace + 1.0f);
		orientation->W() = 0.5f * root;
		root = 0.5f / root;
		orientation->X() = root * (Row[1].z - Row[2].y);
		orientation->Y() = root * (Row[2].x - Row[0].z);
		orientation->Z() = root * (Row[0].y - Row[1].x);
	} 
	else
	{
		static int Next[3] = {1, 2, 0};
		i = 0;
		if(Row[1].y > Row[0].x) i = 1;
		if(Row[2].z > Row[i][i]) i = 2;
		j = Next[i];
		k = Next[j];

		root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + 1.0f);

		// ref operator [] ambiguous with Quaternion*
		Quaternion& ref = *orientation;
		ref[i] = 0.5f * root;
		root = 0.5f / root;
		ref[j] = root * (Row[i][j] + Row[j][i]);
		ref[k] = root * (Row[i][k] + Row[k][i]);
		ref.W() = root * (Row[j][k] - Row[k][j]);
	} 

	return true;
}



void SceneGraph::Update(const List<Entity*>& entities)
{
    for (uint32_t i = 0; i < entities.GetSize(); i++)
    {
        UpdateTransform(*entities[i]);
    }
}

Matrix SceneGraph::GetTrs(const Matrix& trs,const Entity* entity)
{
    if(!entity->HasParent())
        return trs;
    
    return GetTrsOfParents(*entity->GetParent()) * trs;
}

void SceneGraph::OnAttachToParent(Entity& entity)
{
    Transform& transform = entity.transform;

    Matrix trs = transform.worldMatrix;
    Matrix&& parent = GetTrsOfParents(*entity.GetParent());
    
    trs = parent.Inverted() * trs ;
	Vector3 skew;
	Vector4 perspective;
	
    Decompose(trs,&transform.position,&transform.quaternion,&transform.scale,&skew,&perspective);
	transform.position = static_cast<Vector3>(trs[3]);
	transform.rotation = Quaternion::ToEuler(transform.quaternion);
}

Matrix SceneGraph::GetTrsOfParents(const Entity& parent)
{
    Matrix&& parentMatrix = Matrix::Trs(parent.transform.position,parent.transform.quaternion,parent.transform.scale);

    if(parent.HasParent())  
    {
        return GetTrsOfParents(*parent.GetParent()) * parentMatrix;
    }
    return parentMatrix;
}

void SceneGraph::UpdateTransform(Entity& entity)
{
	
    entity.transform.quaternion = Quaternion::FromEuler(entity.transform.rotation).Normalized();
    entity.transform.worldMatrix = Matrix::Trs(entity.transform.position,entity.transform.quaternion,entity.transform.scale);
    
    if(!entity.HasParent())
        return;
	
    entity.transform.worldMatrix = GetTrsOfParents(*entity.GetParent()) *  entity.transform.worldMatrix;
}
