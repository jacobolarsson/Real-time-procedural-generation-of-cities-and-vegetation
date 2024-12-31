#pragma once
#include "../Common.h"
#include "../Mesh/Mesh.h"

#include <memory>

namespace CityGen
{
	enum class MeshType
	{
		CUBE,
		TREE,
		PLANE,
		BUILDING
	};

	class GameObject
	{
	public:
		GameObject(GridCoords p, unsigned sceneSeed)
			: m_pos(p)
			, m_mesh(nullptr)
			, m_sceneSeed(sceneSeed)
		{}

		void CreateMesh(MeshType meshType, vec3 posInCell = { 0.5f, 0.0f, 0.5f });

		GridCoords GetPos() const { return m_pos; }
		std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
		unsigned GetSceneSeed() const { return m_sceneSeed; }

	private:
		GridCoords m_pos;
		std::shared_ptr<Mesh> m_mesh;
		unsigned m_sceneSeed;
	};

}
