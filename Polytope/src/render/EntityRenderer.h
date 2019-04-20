#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "MeshBuffer.h"
#include "ShaderProgram.h"
#include <world/Entity.h>

struct DrawCall {
	MeshOffset offset;
	glm::mat4 transform;
};

class EntityRenderer {

public:
	EntityRenderer(ShaderProgram& shader);
	~EntityRenderer();

	void upload_meshes(std::vector<Mesh>& meshes);

	void draw_entity(const Entity& entity);
	void render();

private:
	GLuint init_vao();

	GLuint m_vao = 0;
	MeshBuffer m_meshes;
	//for now just have it as a class var, we'll deal with materials later
	ShaderProgram& m_shader;
	std::vector<DrawCall> m_draw_calls;

};

