#include "Window/Window.h"
#include "Scene/Scene.h"
#include "Shader/Shader.h"
#include "Shapes/Shapes.h"

using namespace CityGen;

#undef main
int main(void)
{
	static const int width = 1920;
	static const int height = 1080;

	Window window(width, height);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	bool quit = false;
	SDL_Event event;
	Scene scene;

	GLuint shader = ShaderHelpers::CreateShaderProgram("data/shaders/vertex.vert",
													   "data/shaders/fragment.frag");
	size_t now = SDL_GetTicks64();
	size_t last = 0;

	Mesh::LoadVtxData("data/models/tree_trunk.obj", "tree_trunk");
	Mesh::LoadVtxData("data/models/sphere.obj", "sphere");
	ShapeUtils::CreateShapes();

	while (!quit) {
		last = now;
		now = SDL_GetTicks64();
		float deltaT = static_cast<float>(now - last) * 0.001f;

		window.PollEvents(&event, scene.GetCamera(), quit, deltaT);
		scene.Render(shader, window, deltaT);
	}

	return 0;
}
