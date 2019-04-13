#pragma once

#include <string>
#include <glad/glad.h>

class ShaderProgram {

public:
	ShaderProgram(const std::string& vertex_code, const std::string& frag_code);
	~ShaderProgram();

	void use();

private:
	GLuint compile_shader(const std::string& code, GLenum shader_type);
	GLuint link_shaders(GLuint vert_shader, GLuint frag_shader);

	GLuint m_vert_shader = 0;
	GLuint m_frag_shader = 0;
	GLuint m_program = 0;
};

