#pragma once

#include "../Camera/Camera.h"
#include "../GameObject/GameObject.h"
#include "../Window/Window.h"
#include "../LRUCache/LRUCache.h"

#include <chrono>

namespace CityGen
{
	class Scene
	{
	public:
		Scene()
			: m_renderDist(8)
			, m_cache(1000)
			, m_cam(vec3(0.0f, 0.5f, 0.0f))
			, m_dirLight(glm::normalize(vec3(1.1234f, -1.6558f, -1.897f)))
			, m_seed(std::chrono::system_clock::now().time_since_epoch().count())
		{}

		void Render(GLuint shader, Window const& window, float deltaT);
		void PassUniforms(GLuint shader) const;

		Camera& GetCamera() { return m_cam; }

	private:
		void DrawMeshes(std::vector<std::shared_ptr<Mesh>> const& meshData) const;

		int m_renderDist;
		LRUCache m_cache;
		Camera m_cam;
		vec3 m_dirLight;
		unsigned m_seed;
	};

}
