#include "LRUCache.h"

#include <iostream>

using namespace CityGen;

std::vector<std::shared_ptr<Mesh>> const& LRUCache::Get(GridCoords pos)
{
	auto mapNode = m_iteratorMap.find(Key(pos));
	if (mapNode == m_iteratorMap.end()) {
		std::cerr << "LRUCache error: trying to access an non-existent element in the cache\n";
	}
	// Move the mesh nodes to the front
	m_meshDataList.splice(m_meshDataList.begin(), m_meshDataList, mapNode->second);

	return m_meshDataList.front().first;
}

void CityGen::LRUCache::PushFront(std::vector<std::shared_ptr<Mesh>> const& meshes, GridCoords pos)
{
	size_t key = Key(pos);
	if (m_iteratorMap.find(key) != m_iteratorMap.end()) {
		std::cerr << "LRUCache error: trying to push an element that is already in the cache\n";
	}

	// If cache is full, delete lest recently used mesh node
	if (m_meshDataList.size() == m_maxSize) {
		m_iteratorMap.erase(m_meshDataList.back().second);
		m_meshDataList.pop_back();
	}
	// Push the new meshes node to the front of the cache
	m_meshDataList.emplace_front(std::make_pair(meshes, key));
	m_iteratorMap.emplace(key, m_meshDataList.begin());
}
