#include "App.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
