#pragma once

#include "../Common.h"

#include <glm/gtc/constants.hpp> // glm::half_pi
#include <glm/gtc/matrix_transform.hpp> // glm::lookAt, glm::perspective 

namespace CityGen
{
	class Camera
	{
	public:
		Camera(vec3 pos)
			: m_pos(pos)
			, m_dir(0.0f, 0.0f, -1.0f)
			, m_fov(glm::half_pi<float>())
			, m_minDotVal(glm::cos(m_fov))
			, m_perspMtx(1.0f)
			, m_w2perspMtx(1.0f)
			, m_pitch(0.0f)
			, m_yaw(0.0f)
		{
			m_perspMtx = glm::perspective(m_fov * 9.0f / 16.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
			mat4 viewMtx = glm::lookAt(m_pos, m_pos + m_dir, vec3(0.0f, 1.0f, 0.0f));
			m_w2perspMtx = m_perspMtx * viewMtx;
		}

		void Update(float deltaT);
		void HandleMouseInput(SDL_Event* event, float deltaT);
		void HandleKeyboardInput(float deltaT);
		bool CanSee(GridCoords cell);

		mat4 const& GetWorld2PerspMtx() const { return m_w2perspMtx; }
		vec3 GetPos() const { return m_pos; }

	private:
		vec3 m_pos;
		vec3 m_dir;
		float m_fov;
		float m_minDotVal;
		mat4 m_perspMtx;
		mat4 m_w2perspMtx;
		float m_pitch;
		float m_yaw;
	};

}
