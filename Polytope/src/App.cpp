#include "App.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cereal/archives/binary.hpp>

#include <polytope_tools/Mesh.h>

#include "render/MeshBuffer.h"
#include "render/ShaderProgram.h"

bool App::init_GL() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
		std::cout << "OpenGL context initialization failed.\n";
		return false;
	}
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	return true;
}

bool App::init_GLFW() {
	if (glfwInit() == false) {
		std::cout << "GLFW initialization failed.\n";
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	m_wnd = glfwCreateWindow(m_width, m_height, "Polytope", NULL, NULL);
	if (m_wnd == nullptr) {
		std::cout << "Window creation failed.\n";
		return false;
	}
	glfwMakeContextCurrent(m_wnd);
	return true;
}

bool App::init() {
	if (init_GLFW() == false) {
		return false;
	}
	if (init_GL() == false) {
		return false;
	}
	return true;
}

void App::start() {
	//just load it here for now
	std::ifstream file("data/mesh/suzanne.mdl", std::ios::binary);
	cereal::BinaryInputArchive archive(file);
	Mesh mesh;
	archive(mesh);
	MeshBuffer mesh_buffer;
	std::vector<Mesh> meshes = { mesh };
	mesh_buffer.upload_meshes(meshes);
	//load the shader
	std::ifstream vert_file("data/shader/basic.vert");
	std::ifstream frag_file("data/shader/basic.frag");
	std::stringstream vert_stream;
	vert_stream << vert_file.rdbuf();
	std::stringstream frag_stream;
	frag_stream << frag_file.rdbuf();
	ShaderProgram program(vert_stream.str(), frag_stream.str());
	program.use();
	while (!glfwWindowShouldClose(m_wnd)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(m_wnd);
	}
}

App::App() {
}

App::~App() {
}
