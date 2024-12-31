#pragma once
#include "../Common.h"

#include <vector>
#include <array>

namespace ShapeUtils
{
	void CreateShapes();
	void CreateSquare(size_t idx, float width, float length);
	void CreatePolygon(size_t idx, float radius, int sideCount);
	std::array<std::vector<vec3>, 4> const& GetShapes();
}
