#pragma once
#include "../Common.h"
#include "../Mesh/Mesh.h"

#include <list>
#include <unordered_map>

namespace CityGen
{
	using MeshList = std::list<std::pair<std::vector<std::shared_ptr<Mesh>>, size_t>>;

	class LRUCache
	{
	public:
		LRUCache(size_t size) : m_maxSize(size) {}

		std::vector<std::shared_ptr<Mesh>> const& Get(GridCoords pos);
		void PushFront(std::vector<std::shared_ptr<Mesh>> const& meshes, GridCoords pos);

		bool Contains(GridCoords pos) const { return m_iteratorMap.find(Key(pos)) != m_iteratorMap.end(); }
		std::vector<std::shared_ptr<Mesh>> const& Front() const { return m_meshDataList.front().first; }

	private:
		inline size_t Key(GridCoords pos) const { return (static_cast<size_t>(pos.x) << 32) |
														  static_cast<unsigned>(pos.z); }
		MeshList m_meshDataList;
		std::unordered_map<size_t, MeshList::iterator> m_iteratorMap;
		size_t m_maxSize;
	};

}
