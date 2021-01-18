#include "Entity.h"

#include <render/EntityRenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

Entity::Entity(
	const std::string& mesh_name, 
	const std::string& material_name, 
	glm::vec3 pos,
	glm::vec3 rot
) : m_mesh(mesh_name), m_material(material_name) {
	m_transform = glm::translate(m_transform, pos);
	//blender exported meshes have to be rotated to fit OpenGL's coord system. Eventually, we'll do this
	//on the tools side
	m_transform = glm::rotate(m_transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_transform = glm::eulerAngleXYZ(rot.x, rot.y, rot.z) * m_transform;
}

Entity::~Entity() {
}

void Entity::update(double dt) {
}

void Entity::render(EntityRenderer& renderer) {
	renderer.draw_entity(*this);
}
