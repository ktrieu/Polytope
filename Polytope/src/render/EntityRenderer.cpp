#include "EntityRenderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <polytope_tools/Mesh.h>
#include <render/Camera.h>
#include <resource/ResourceLoader.h>

EntityRenderer::EntityRenderer() {
	m_vao = init_vao();
}

EntityRenderer::~EntityRenderer() {
	glDeleteVertexArrays(1, &m_vao);
}

void EntityRenderer::upload_meshes(std::vector<Mesh>& meshes) {
	m_meshes.upload_meshes(meshes);
}

GLenum get_format_for_texture(const Texture& texture) {
	if (texture.channels_per_pixel == 3) {
		return GL_RGB;
	}
	else if (texture.channels_per_pixel == 4) {
		return GL_RGBA;
	}
}

void EntityRenderer::upload_textures(std::vector<Texture>& textures) {
	for (const auto& texture : textures) {
		GLuint texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, get_format_for_texture(texture), GL_UNSIGNED_BYTE, texture.data.data());
		std::cout << glGetError() << '\n';
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_texture_ids.emplace(texture.name, texture_id);
	}
}

void EntityRenderer::draw_entity(const Entity& entity) {
	MeshOffset offset = m_meshes.get_mesh_offset(entity.get_mesh());
	m_draw_calls.push_back(DrawCall{ offset, entity.get_material(), entity.get_transform() });
}

void EntityRenderer::render(Camera& camera, ResourceLoader& loader) {
	glBindVertexArray(m_vao);
	glm::mat4 view = camera.get_view();
	glm::mat4 proj = camera.get_proj();
	for (DrawCall& call : m_draw_calls) {
		Material& mat = loader.get_material(call.material);
		ShaderProgram& shader = loader.get_shader(mat.shader_name);
		shader.use();
		glBindTexture(GL_TEXTURE_2D, m_texture_ids[mat.texture_name]);
		glm::mat4 model = call.transform;
		shader.uploadUniform(model, "model");
		shader.uploadUniform(view, "view");
		shader.uploadUniform(proj, "projection");
		shader.uploadUniform(0, "tex_sampler");
		glDrawElementsBaseVertex(GL_TRIANGLES, call.offset.index_len, GL_UNSIGNED_SHORT,
			(void*)(call.offset.index_offset * sizeof(unsigned short)), call.offset.base_vertex);
	}
	glBindVertexArray(0);
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glBindVertexArray(0);
	return vao_id;
}
