#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderProgram {

public:
	ShaderProgram(const std::string& vertex_code, const std::string& frag_code);
	~ShaderProgram();

	void use();
	void unuse();

	void uploadUniform(const glm::mat4& mat, const std::string& name);
	void uploadUniform(const int value, const std::string& name);

private:
	GLuint compile_shader(const std::string& code, GLenum shader_type);
	GLuint link_shaders(GLuint vert_shader, GLuint frag_shader);

	bool m_active = false;
	GLuint m_vert_shader = 0;
	GLuint m_frag_shader = 0;
	GLuint m_program = 0;
};

