#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "MeshBuffer.h"
#include "ShaderProgram.h"
#include <world/Entity.h>
#include <world/Light.h>
#include <render/Camera.h>
#include <render/ShadowMap.h>
#include <polytope_tools/Material.h>
#include <polytope_tools/Texture.h>

class ResourceLoader;

struct DrawCall {
	MeshOffset offset;
	std::string material;
	glm::mat4 transform;

	DrawCall(MeshOffset offset, std::string material, glm::mat4 transform) 
		: offset(offset), material(material), transform(transform) {}
};

class EntityRenderer {

public:
	EntityRenderer(ResourceLoader& loader);
	~EntityRenderer();

	void upload_meshes(std::vector<Mesh>& meshes);
	void upload_textures(std::vector<Texture>& textures);

	void draw_entity(const Entity& entity);
	void draw_light(const Light& light);
	void render(Camera& camera);

private:
	GLuint init_vao();

	void prepare_forward_shader(ShaderProgram& shader, Material& mat, glm::mat4& view, glm::mat4& proj);
	void upload_light_uniforms(ShaderProgram& shader, Light& light, glm::mat4& view, int light_idx);
	void execute_draw_call(ShaderProgram& shader, DrawCall& draw_call);

	ResourceLoader& m_loader;

	ShadowMap m_shadow_map;

	GLuint m_vao = 0;
	MeshBuffer m_meshes;
	std::unordered_map<std::string, GLuint> m_texture_ids;

	std::unordered_map<std::string, std::vector<DrawCall>> m_draw_calls;
	std::vector<Light> m_lights;

};

