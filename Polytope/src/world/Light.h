#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	glm::mat4 get_view() {
		return glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::mat4 get_proj() {
		return glm::perspective(fov, 1.0f, 0.01f, 100.0f);
	}
};