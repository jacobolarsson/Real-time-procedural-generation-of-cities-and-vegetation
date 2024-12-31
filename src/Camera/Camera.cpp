#include "Camera.h"

using namespace CityGen;

void Camera::Update(float deltaT)
{
	// Compute camera direction
	m_dir.x = glm::cos(m_yaw) * glm::cos(m_pitch);
	m_dir.y = glm::sin(m_pitch);
	m_dir.z = glm::sin(m_yaw) * glm::cos(m_pitch);
	m_dir = glm::normalize(m_dir);

	HandleKeyboardInput(deltaT);

	mat4 viewMtx = glm::lookAt(m_pos, m_pos + m_dir, vec3(0.0f, 1.0f, 0.0f));
	m_w2perspMtx = m_perspMtx * viewMtx;
}

void Camera::HandleMouseInput(SDL_Event* event, float deltaT)
{
	static const float sensitivity = 0.3f;
	static const float halfPi = glm::half_pi<float>();

	m_yaw += event->motion.xrel * sensitivity * deltaT;

	if (m_pitch > halfPi - 0.1f) {
		m_pitch = halfPi - 0.1f;
		return;
	} if (m_pitch < -halfPi + 0.1f) {
		m_pitch = -halfPi + 0.1f;
		return;
	}

	m_pitch -= event->motion.yrel * sensitivity * deltaT;
}

void Camera::HandleKeyboardInput(float deltaT)
{
	// Do not let the player get under the ground
	if (m_pos.y <= 0.1f) {
		m_pos.y = 0.1f;
	}

	static const float moveSpeed = 1.0f;
	vec3 right = glm::normalize(glm::cross(m_dir, vec3(0.0f, 1.0f, 0.0f)));

	const unsigned char* keyStates = SDL_GetKeyboardState(nullptr);
	if (keyStates[SDL_SCANCODE_W]) {
		m_pos += m_dir * moveSpeed * deltaT;
	} if (keyStates[SDL_SCANCODE_A]) {
		m_pos -= right * moveSpeed * deltaT;
	} if (keyStates[SDL_SCANCODE_S]) {
		m_pos -= m_dir * moveSpeed * deltaT;
	} if (keyStates[SDL_SCANCODE_D]) {
		m_pos += right * moveSpeed * deltaT;
	} if (keyStates[SDL_SCANCODE_SPACE]) {
		m_pos += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * deltaT;
	} if (keyStates[SDL_SCANCODE_LSHIFT]) {
		m_pos -= vec3(0.0f, 1.0f, 0.0f) * moveSpeed * deltaT;
	}
}

bool Camera::CanSee(GridCoords cell)
{
	vec2 posVec2 = vec2(m_pos.x, m_pos.z);
	vec2 dirVec2 = vec2(m_dir.x, m_dir.z);

	vec2 corner1 = vec2(static_cast<float>(cell.x), static_cast<float>(cell.z));
	vec2 corner2 = vec2(static_cast<float>(cell.x) + 1.0f, static_cast<float>(cell.z));
	vec2 corner3 = vec2(static_cast<float>(cell.x), static_cast<float>(cell.z) + 1.0f);
	vec2 corner4 = vec2(static_cast<float>(cell.x) + 1.0f, static_cast<float>(cell.z) + 1.0f);
	// Cell is visible if any of it's corners is visible
	return glm::dot(glm::normalize(corner1 - posVec2), dirVec2) > m_minDotVal || 
		   glm::dot(glm::normalize(corner2 - posVec2), dirVec2) > m_minDotVal ||
		   glm::dot(glm::normalize(corner3 - posVec2), dirVec2) > m_minDotVal ||
		   glm::dot(glm::normalize(corner4 - posVec2), dirVec2) > m_minDotVal;
}
