#pragma once
#include "../Common.h"
#include "../Camera/Camera.h"

#include <iostream>

namespace CityGen
{
	class Window
	{
	public:
		Window(int width, int height);
		~Window();

		void PollEvents(SDL_Event* Event, Camera& camera, bool& quit, float deltaT);

		SDL_Window* GetSDLWindow() const { return m_window; }

	private:
		SDL_Window* m_window;
		SDL_GLContext m_context;
	};

}
