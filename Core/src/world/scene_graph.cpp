#include "world/scene_graph.hpp"

#include <Maths/matrix.hpp>

using namespace XnorCore;

Matrix GetTrsOfParents(const Entity& parent)
{
	const Matrix parentMatrix = Matrix::Trs(parent.transform.position, parent.transform.rotation, parent.transform.scale);

	if (parent.HasParent())  
	{
		return GetTrsOfParents(*parent.GetParent()) * parentMatrix;
	}
	
	return parentMatrix;
}

void SceneGraph::UpdateTransform(Entity& entity)
{
	Transform& t = entity.transform;

	if (!t.changed)
		return;
	
	t.changed = false;
	t.rotation = Quaternion::FromEuler(t.eulerRotation).Normalized();
	t.worldMatrix = Matrix::Trs(t.position, t.rotation, t.scale);
		
	if (entity.HasParent())
	{
		t.worldMatrix = GetTrsOfParents(*entity.GetParent()) *  t.worldMatrix;
	}

	if (!entity.HasChildren())
		return;
		
	for (size_t i = 0; i < entity.GetChildCount(); i++)
	{
		Entity& ent = *entity.GetChild(i);
		ent.transform.changed = true;
		UpdateTransform(ent);
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
	
    trs.Decompose(&transform.position, &transform.rotation, &transform.scale, &skew, &perspective);
	transform.position = static_cast<Vector3>(trs[3]);
	transform.eulerRotation = Quaternion::ToEuler(transform.rotation);
}
