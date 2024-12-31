#include "GameObject.h"

using namespace CityGen;

void GameObject::CreateMesh(MeshType meshType, vec3 posInCell /*= { 0.5f, 0.0f, 0.5f }*/)
{
	switch (meshType) {
	case MeshType::CUBE: {
		m_mesh = std::make_shared<Cube>(this, m_pos, m_sceneSeed);
		break;
	} case MeshType::TREE: {
		m_mesh = std::make_shared<Tree>(this, m_pos, m_sceneSeed, posInCell);
		break;
	} case MeshType::PLANE: {
		m_mesh = std::make_shared<Plane>(this, m_pos, m_sceneSeed);
		break;
	} case MeshType::BUILDING: {
		m_mesh = std::make_shared<Building>(this, m_pos, m_sceneSeed);
		break;
	}
	}
}
