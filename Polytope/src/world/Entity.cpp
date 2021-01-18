#include "Entity.h"

#include <render/EntityRenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

Entity::Entity(
	const std::string& mesh_name, 
	const std::string& material_name, 
	glm::vec3 pos,
	glm::vec3 rot
) : m_mesh(mesh_name), m_material(material_name), m_pos(pos), m_rot(rot) {
	recalculate_transform();
}

Entity::~Entity() {
}

void Entity::recalculate_transform() {
	m_transform = glm::mat4(1.0f);
	m_transform = glm::translate(m_transform, m_pos);
	//blender exported meshes have to be rotated to fit OpenGL's coord system. Eventually, we'll do this
	//on the tools side
	m_transform = glm::rotate(m_transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_transform = glm::eulerAngleXYZ(m_rot.x, m_rot.y, m_rot.z) * m_transform;
}

void Entity::update(double dt) {
	// this is a huge hack, but I do not want to deal with polymorphism or proper transforms right now
	if (m_mesh.compare("arena_scene/cube/cube") == 0) {
		glm::mat4 rotation(1.0f);
		rotation = glm::translate(rotation, -m_pos);
		rotation = glm::rotate(rotation, glm::radians(0.01f) * (float)dt, glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::translate(rotation, m_pos);
		m_pos = rotation * glm::vec4(m_pos, 1.0f);
		recalculate_transform();
	}
}

void Entity::render(EntityRenderer& renderer) {
	renderer.draw_entity(*this);
}
