#include "ShaderProgram.h"

#include <vector>
#include <stdexcept>

ShaderProgram::ShaderProgram(const std::string& vertex_code, const std::string& frag_code) {
	m_vert_shader = compile_shader(vertex_code, GL_VERTEX_SHADER);
	m_frag_shader = compile_shader(frag_code, GL_FRAGMENT_SHADER);
	m_program = link_shaders(m_vert_shader, m_frag_shader);
}

ShaderProgram::~ShaderProgram() {
	glDeleteShader(m_vert_shader);
	glDeleteShader(m_frag_shader);
	glDeleteProgram(m_program);
}

void ShaderProgram::use() {
	glUseProgram(m_program);
}

GLuint ShaderProgram::compile_shader(const std::string& code, GLenum shader_type) {
	GLuint shader_id = glCreateShader(shader_type);
	//unfortunately glShaderSource wants arrays of string and lengths, so we have to do some gymnastics
	const char* code_ptr = code.data();
	int code_length = code.size();
	glShaderSource(shader_id, 1, &code_ptr, &code_length);
	glCompileShader(shader_id);
	//check for errors
	GLint compile_status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		int log_length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> log(log_length);
		glGetShaderInfoLog(shader_id, log_length, &log_length, log.data());
		std::string log_str(log.begin(), log.end());
		glDeleteShader(shader_id);
		std::string error_str;
		if (shader_type == GL_VERTEX_SHADER) {
			error_str = "Vertex shader compile error: " + log_str;
		}
		else if (shader_type == GL_FRAGMENT_SHADER) {
			error_str = "Fragment shader compile error: " + log_str;
		}
		throw std::runtime_error(error_str);
	}
	return shader_id;
}

GLuint ShaderProgram::link_shaders(GLuint vert_shader, GLuint frag_shader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	int is_linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE) {
		int log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> log(log_length);
		glGetProgramInfoLog(program, log_length, &log_length, log.data());
		glDeleteProgram(program);
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);
		throw std::runtime_error("Program link error: " + std::string(log.begin(), log.end()));
	}
	glDetachShader(program, vert_shader);
	glDetachShader(program, frag_shader);
	return program;
}
