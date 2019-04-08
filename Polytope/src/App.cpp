#include "App.h"

#include <iostream>

#include <GLFW/glfw3.h>

bool App::init_GLFW() {
	if (glfwInit() == false) {
		std::cout << "GLFW initialization failed.\n";
		return false;
	}
	m_wnd = glfwCreateWindow(m_width, m_height, "Polytope", NULL, NULL);
	if (m_wnd == nullptr) {
		std::cout << "Window creation failed.\n";
		return false;
	}
	return true;
}

bool App::init() {
	return init_GLFW();
}

void App::start() {
	while (!glfwWindowShouldClose(m_wnd)) {
		glfwPollEvents();
		glfwSwapBuffers(m_wnd);
	}
}

App::App() {
}

App::~App() {
}
