#include "EntityRenderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <polytope_tools/Mesh.h>
#include <render/Camera.h>
#include <resource/ResourceLoader.h>

EntityRenderer::EntityRenderer(ResourceLoader& loader) : m_loader(loader) {
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
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_texture_ids.emplace(texture.name, texture_id);
	}
}

void EntityRenderer::draw_entity(const Entity& entity) {
	MeshOffset offset = m_meshes.get_mesh_offset(entity.get_mesh());
	auto material_draw_calls = m_draw_calls.find(entity.get_material());

	DrawCall new_draw_call(offset, entity.get_material(), entity.get_transform());

	if (material_draw_calls != m_draw_calls.end()) {
		material_draw_calls->second.push_back(new_draw_call);
	}
	else {
		m_draw_calls.emplace(entity.get_material(), std::vector<DrawCall>{ new_draw_call });
	}
}

void EntityRenderer::draw_light(const Light& light) {
	m_lights.push_back(light);
}

void EntityRenderer::render(Camera& camera) {
	glBindVertexArray(m_vao);
	glm::mat4 view = camera.get_view();
	glm::mat4 proj = camera.get_proj();

	for (auto& material_draw_calls : m_draw_calls) {
		Material& mat = m_loader.get_material(material_draw_calls.first);
		ShaderProgram& shader = m_loader.get_shader(mat.shader_name);
		shader.use();
		glBindTexture(GL_TEXTURE_2D, m_texture_ids[mat.texture_name]);
		shader.uploadUniform(view, "view");
		shader.uploadUniform(proj, "projection");
		shader.uploadUniform(0, "tex_sampler");
		shader.uploadUniform(static_cast<int>(m_lights.size()), "num_lights");
		for (int i = 0; i < m_lights.size(); i++) {
			Light light = m_lights.at(i);
			std::string light_struct_name = "lights[" + std::to_string(i) + "]";
			glm::vec3 light_pos_view_space = view * glm::vec4(light.pos, 1.0);
			glm::vec3 light_dir_view_space = view * glm::vec4(light.dir, 0.0);
			shader.uploadUniform(light_pos_view_space, light_struct_name + ".pos");
			shader.uploadUniform(light.color, light_struct_name + ".color");
			shader.uploadUniform(light_dir_view_space, light_struct_name + ".dir");
			shader.uploadUniform(light.strength, light_struct_name + ".strength");
			shader.uploadUniform(light.fov, light_struct_name + ".fov");
		}

		for (auto& draw_call : material_draw_calls.second) {
			glm::mat4 model = draw_call.transform;
			shader.uploadUniform(model, "model");
			glDrawElementsBaseVertex(GL_TRIANGLES, draw_call.offset.index_len, GL_UNSIGNED_SHORT,
				(void*)(draw_call.offset.index_offset * sizeof(unsigned short)), draw_call.offset.base_vertex);
		}

		shader.unuse();
	}
	glBindVertexArray(0);
	m_draw_calls.clear();
	m_lights.clear();
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
