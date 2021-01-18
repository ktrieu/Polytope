#pragma once

#include <glm/glm.hpp>

struct Light {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 dir;

	float fov;
	float strength;

	Light(
		glm::vec3 pos,
		glm::vec3 color,
		glm::vec3 dir,
		float fov, 
		float strength
	) : 
		pos(pos),
		color(color), 
		dir(glm::normalize(dir)), 
		fov(fov), 
		strength(strength)
	{}
};