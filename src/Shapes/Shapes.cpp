#include "Shapes.h"

#include <glm/gtc/constants.hpp> // glm::half_pi
#include <glm/gtc/matrix_transform.hpp> // glm::lookAt, glm::perspective 

namespace ShapeUtils
{
	static std::array<std::vector<vec3>, 4> shapes{};
}

void ShapeUtils::CreateShapes()
{
	CreateSquare(0, 1.0f, 1.0f);
	CreateSquare(1, 0.7f, 1.3f);
	CreatePolygon(2, 0.8f, 5);
	CreatePolygon(3, 0.6f, 9);
}

void ShapeUtils::CreateSquare(size_t idx, float width, float length)
{
	float halfWidth = width / 2.0f;
	float halfLength = length / 2.0f;

	shapes[idx].emplace_back(-halfWidth, 0.0f,  halfLength);
	shapes[idx].emplace_back(-halfWidth, 0.0f, -halfLength);
	shapes[idx].emplace_back( halfWidth, 0.0f, -halfLength);
	shapes[idx].emplace_back( halfWidth, 0.0f,  halfLength);
}

void ShapeUtils::CreatePolygon(size_t idx, float radius, int sideCount)
{
	vec3 currentPoint(0.0f, 0.0f, radius);
	float angleStep = glm::two_pi<float>() / static_cast<float>(sideCount);
	mat4 rot = glm::rotate(mat4(1.0f), -angleStep, vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < sideCount; i++) {
		currentPoint = vec3(rot * vec4(currentPoint, 1.0f));
		shapes[idx].emplace_back(currentPoint);
	}
}

std::array<std::vector<vec3>, 4> const& ShapeUtils::GetShapes()
{
	return shapes;
}
