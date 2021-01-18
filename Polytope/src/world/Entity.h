#pragma once

#include <glm/glm.hpp>

#include <polytope_tools/Mesh.h>

class EntityRenderer;

class Entity {

public:
	Entity(const std::string& mesh_name, const std::string& material_name, glm::vec3 pos, glm::vec3 rot);
	~Entity();

	const std::string& get_mesh() const { return m_mesh; }
	const std::string& get_material() const { return m_material; }
	glm::mat4 get_transform() const { return m_transform; }

	void update();
	void render(EntityRenderer& renderer);

private:
	glm::mat4 m_transform = glm::mat4(1.0f);
	std::string m_mesh;
	std::string m_material;

};

