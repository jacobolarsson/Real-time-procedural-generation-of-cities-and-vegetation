#include "Window.h"

using namespace CityGen;

Window::Window(int width, int height)
{
	// Init	SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}
	// Create window
	m_window = SDL_CreateWindow("CityGen", 100, 100, width, height, SDL_WINDOW_OPENGL);
	if (!m_window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	// Disable depricated functions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Create OpenGl context
	m_context = SDL_GL_CreateContext(m_window);
	if (!m_context) {
		SDL_DestroyWindow(m_window);
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	// Init glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		std::cerr << "GLEW Error: Failed to init" << std::endl;
		SDL_Quit();
		exit(1);
	}

}

Window::~Window()
{
	//SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::PollEvents(SDL_Event* Event, Camera& camera, bool& quit, float deltaT)
{
	while (SDL_PollEvent(Event)) {
		switch (Event->type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (Event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
			}
			break;
		case SDL_MOUSEMOTION:
			camera.HandleMouseInput(Event, deltaT);
			break;
		}
	}
}
