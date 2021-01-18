#pragma once

#include <glm/glm.hpp>

struct Light {
	glm::vec3 pos;
	glm::vec3 color;
	float strength;

	Light(glm::vec3 pos, glm::vec3 color, float strength) : pos(pos), color(color), strength(strength) {}
};