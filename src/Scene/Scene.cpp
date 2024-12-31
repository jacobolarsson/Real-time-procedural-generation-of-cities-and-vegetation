#include "Scene.h"

using namespace CityGen;

void Scene::Render(GLuint shader, Window const& window, float deltaT)
{
	m_cam.Update(deltaT);
	//Clear screen and depth buffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader);
	PassUniforms(shader);

	GridCoords camGridPos = { static_cast<int>(m_cam.GetPos().x),
							  static_cast<int>(m_cam.GetPos().z) };
	for (int i = -m_renderDist + 1; i < m_renderDist; i++) {
		int r = camGridPos.z + i;
		for (int j = -m_renderDist + 1; j < m_renderDist; j++) {
			int c = camGridPos.x + j;
			GridCoords cell = { c, r };
			// If the position is not visible by the camera, skip
			if (!m_cam.CanSee(cell)) {
				continue;
			}
			// If the mesh is already in the list, draw without creating the mesh again
			if (m_cache.Contains(cell)) {
				DrawMeshes(m_cache.Get(cell));
				continue;
			}
			std::vector<std::shared_ptr<Mesh>> meshes;

			if (r % 2 == 0 || c % 6 == 0) {
				if (r % 2 != 0 || c % 6 != 0) {
					GameObject tree1(cell, m_seed);
					GameObject tree2(cell, m_seed);
					tree1.CreateMesh(MeshType::TREE, vec3(0.5f, 0.0, 0.2f));
					tree2.CreateMesh(MeshType::TREE, vec3(0.5f, 0.0, 0.7f));
					meshes.push_back(tree1.GetMesh());
					meshes.push_back(tree2.GetMesh());
				}
				GameObject street(cell, m_seed);
				street.CreateMesh(MeshType::PLANE);
				meshes.push_back(street.GetMesh());

				m_cache.PushFront(meshes, cell);
				DrawMeshes(m_cache.Front());
			} else {
				GameObject building(cell, m_seed);
				building.CreateMesh(MeshType::BUILDING);
				meshes.push_back(building.GetMesh());

				m_cache.PushFront(meshes, cell);
				DrawMeshes(m_cache.Front());
			}

		}
	}
	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);

	SDL_GL_SwapWindow(window.GetSDLWindow());
}

void Scene::PassUniforms(GLuint shader) const
{
	// Pass the world to ndc matrix to the shader
	GLint loc = glGetUniformLocation(shader, "w2PerspMtx");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m_cam.GetWorld2PerspMtx()[0][0]);

	// Pass the directional lights
	loc = glGetUniformLocation(shader, "lightDir1");
	glUniform3f(loc, -m_dirLight.x, -m_dirLight.y, -m_dirLight.z);

	loc = glGetUniformLocation(shader, "lightDir2");
	vec3 oppositeLight = m_dirLight * 0.3f;
	glUniform3f(loc, oppositeLight.x, oppositeLight.y, oppositeLight.z);
}

void Scene::DrawMeshes(std::vector<std::shared_ptr<Mesh>> const& meshData) const
{
	int vtxCount = 0;
	for (auto mesh : meshData) {
		glBindVertexArray(mesh->GetVao());
		vtxCount = static_cast<int>(mesh->GetVtxCount());
		glDrawArrays(GL_TRIANGLES, 0, vtxCount);
	}
}
