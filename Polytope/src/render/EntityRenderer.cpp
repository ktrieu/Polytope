#include "EntityRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include <polytope_tools/Mesh.h>
#include <render/Camera.h>

EntityRenderer::EntityRenderer() {
}

EntityRenderer::~EntityRenderer() {
	glDeleteVertexArrays(1, &m_vao);
}

void EntityRenderer::load_shader(ShaderProgram& shader) {
	m_shader = &shader;
	m_vao = init_vao();
}

void EntityRenderer::upload_meshes(std::vector<Mesh>& meshes) {
	m_meshes.upload_meshes(meshes);
}

void EntityRenderer::draw_entity(const Entity& entity) {
	MeshOffset offset = m_meshes.get_mesh_offset(entity.get_mesh().name);
	m_draw_calls.push_back(DrawCall{ offset, entity.get_transform() });
}

void EntityRenderer::render(Camera& camera) {
	if (m_shader == nullptr) {
		throw std::runtime_error("No shader is loaded. Call load_shader()");
	}
	m_shader->use();
	glBindVertexArray(m_vao);
	glm::mat4 view = camera.get_view();
	glm::mat4 proj = camera.get_proj();
	m_shader->uploadUniform(view, "view");
	m_shader->uploadUniform(proj, "projection");
	for (DrawCall& call : m_draw_calls) {
		glm::mat4 model = call.transform;
		m_shader->uploadUniform(model, "model");
		glDrawElementsBaseVertex(GL_TRIANGLES, call.offset.index_len, GL_UNSIGNED_SHORT,
			(void*)(call.offset.index_offset * sizeof(unsigned short)), call.offset.base_vertex);
	}
	glBindVertexArray(0);
	m_shader->unuse();
	m_draw_calls.clear();
}

GLuint EntityRenderer::init_vao() {
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	m_meshes.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
	glBindVertexArray(0);
	return vao_id;
}
