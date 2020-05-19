#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "MeshBuffer.h"
#include "ShaderProgram.h"
#include <world/Entity.h>
#include <render/Camera.h>
#include <polytope_tools/Material.h>

class ResourceLoader;

struct DrawCall {
	MeshOffset offset;
	std::string material;
	glm::mat4 transform;
};

class EntityRenderer {

public:
	EntityRenderer();
	~EntityRenderer();

	void upload_meshes(std::vector<Mesh>& meshes);

	void draw_entity(const Entity& entity);
	void render(Camera& camera, ResourceLoader& loader);

private:
	GLuint init_vao();

	GLuint m_vao = 0;
	MeshBuffer m_meshes;
	std::vector<DrawCall> m_draw_calls;

};

