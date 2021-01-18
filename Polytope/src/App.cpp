#include "App.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cereal/archives/binary.hpp>

#include <polytope_tools/Mesh.h>

#include <world/World.h>

bool App::init_GL() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
		std::cout << "OpenGL context initialization failed.\n";
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glActiveTexture(GL_TEXTURE0);
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

	glfwSetInputMode(m_wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(m_wnd, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	glfwSetCursorPos(m_wnd, 0, 0);

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
	World world(*this);
	world.load(m_resource_loader);
	auto last_time = std::chrono::system_clock::now();
	while (!glfwWindowShouldClose(m_wnd)) {
		auto current_time = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> delta = current_time - last_time;
		last_time = current_time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_input_manager.update(delta.count());

		// hard code exit in here for now
		if (m_input_manager.get_key_state(VirtualKey::EXIT)) {
			glfwSetWindowShouldClose(m_wnd, true);
		}

		world.update(delta.count());
		world.render(m_resource_loader);
		glfwPollEvents();
		glfwSwapBuffers(m_wnd);
	}
}


App::App() : m_resource_loader("data/"), m_input_manager(*this) {}

App::~App() {
}
