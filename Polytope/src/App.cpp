#include "App.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cereal/archives/binary.hpp>

#include <polytope_tools/Mesh.h>

#include <world/World.h>

static void glfw_key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
	App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(wnd));
	app_ptr->get_input_manager().on_glfw_key_event(key, scancode, action, mods);
}

static void glfw_cursor_pos_callback(GLFWwindow* wnd, double x, double y) {
	App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(wnd));
	app_ptr->get_input_manager().on_glfw_cursor_pos_event(x, y);
}

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

	glfwSetWindowUserPointer(m_wnd, this);
	glfwSetKeyCallback(m_wnd, glfw_key_callback);
	glfwSetCursorPosCallback(m_wnd, glfw_cursor_pos_callback);

	glfwSetInputMode(m_wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(m_wnd, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
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
	World world(*this);
	world.load(m_resource_loader);
	m_input_manager.subscribe(world);
	while (!glfwWindowShouldClose(m_wnd)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		world.update();
		world.render(m_resource_loader);
		glfwPollEvents();
		glfwSwapBuffers(m_wnd);
	}
}

void App::on_key_press(VirtualKey key) {
	if (key == VirtualKey::EXIT) {
		glfwSetWindowShouldClose(m_wnd, true);
	}
}

App::App() : m_resource_loader("data/") {
	m_input_manager.subscribe(*this);
}

App::~App() {
}
