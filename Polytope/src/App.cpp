#include "App.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cereal/archives/binary.hpp>

#include <polytope_tools/Mesh.h>

#include "render/MeshBuffer.h"
#include "render/ShaderProgram.h"
#include "render/EntityRenderer.h"

bool App::init_GL() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
		std::cout << "OpenGL context initialization failed.\n";
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return true;
}

bool App::init_GLFW() {
	if (glfwInit() == false) {
		std::cout << "GLFW initialization failed.\n";
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	m_resource_loader.load_from_subdirectory("mesh/");
	m_resource_loader.load_from_subdirectory("shader/");
	EntityRenderer renderer(m_resource_loader.get_shader("shader\\basic"));
	std::vector<Mesh> meshes = { m_resource_loader.get_mesh("mesh\\suzanne") };
	renderer.upload_meshes(meshes);
	std::vector<Entity> entities;
	entities.push_back(Entity(meshes[0], glm::vec3(0.0f, 0.0f, -4.0f)));
	while (!glfwWindowShouldClose(m_wnd)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (Entity& entity : entities) {
			entity.update();
			renderer.draw_entity(entity);
		}
		renderer.render();
		glfwPollEvents();
		glfwSwapBuffers(m_wnd);
	}
}

App::App() : m_resource_loader("data/") {
}

App::~App() {
}
