#include "EntityRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include <polytope_tools/Mesh.h>

EntityRenderer::EntityRenderer(ShaderProgram& shader) : m_shader(shader) {
	m_vao = init_vao();
}


EntityRenderer::~EntityRenderer() {
	glDeleteVertexArrays(1, &m_vao);
}

void EntityRenderer::upload_meshes(std::vector<Mesh>& meshes) {
	m_meshes.upload_meshes(meshes);
}

void EntityRenderer::draw_entity(const Entity& entity) {
	MeshOffset offset = m_meshes.get_mesh_offset(entity.get_mesh().name);
	m_draw_calls.push_back(DrawCall{ offset, entity.get_transform() });
}

void EntityRenderer::render() {
	//just generate a standard projection matrix for now
	glm::mat4 perspective = glm::perspective(glm::radians(60.0f), 720.0f / 480.0f, 0.01f, 100.0f);
	m_shader.use();
	glBindVertexArray(m_vao);
	for (DrawCall& call : m_draw_calls) {
		glm::mat4 mvp = perspective * call.transform;
		m_shader.uploadUniform(mvp, "mvp");
		glDrawElementsBaseVertex(GL_TRIANGLES, call.offset.index_len, GL_UNSIGNED_SHORT,
			(void*)(call.offset.index_offset * sizeof(unsigned short)), call.offset.base_vertex);
	}
	glBindVertexArray(0);
	m_shader.unuse();
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
