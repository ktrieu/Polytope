#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(Mesh& mesh, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) : m_mesh(mesh) {
	m_transform = glm::translate(m_transform, pos);
}

Entity::~Entity() {
}

void Entity::update() {
	//every entity must spin
	m_transform = glm::rotate(m_transform, 0.001f, glm::vec3(0.0f, 0.0f, 1.0f));
}
