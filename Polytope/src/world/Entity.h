#pragma once

#include <glm/glm.hpp>

#include <polytope_tools/Mesh.h>

class Entity {

public:
	Entity(Mesh& mesh, glm::vec3 pos);
	~Entity();

	const Mesh& get_mesh() const { return m_mesh; }
	glm::mat4 get_transform() const { return m_transform; }

	void update();

private:
	glm::mat4 m_transform = glm::mat4(1.0f);
	Mesh& m_mesh;

};

