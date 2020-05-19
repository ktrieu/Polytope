#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(const std::string& mesh_name, const std::string& material_name, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) : m_mesh(mesh_name), m_material(material_name) {
	m_transform = glm::translate(m_transform, pos);
	//blender exported meshes have to be rotated to fit OpenGL's coord system. Eventually, we'll do this
	//on the tools side
	m_transform = glm::rotate(m_transform, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}

Entity::~Entity() {
}

void Entity::update() {
	//every entity must spin
	m_transform = glm::rotate(m_transform, -0.0001f, glm::vec3(0.0f, 0.0f, 1.0f));
}
