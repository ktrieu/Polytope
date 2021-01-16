#include "InputManager.h"

#include <App.h>
#include <input/VirtualKey.h>

#include <GLFW/glfw3.h>

InputManager::InputManager(App& app) : m_app(app) {
	for (auto entry : VIRTUAL_KEY_STRINGS) {
		m_keys.emplace(entry.first, false);
	}
}

InputManager::~InputManager() {}

void InputManager::update(double dt) {
	GLFWwindow* wnd = m_app.get_window();

	double mouse_x, mouse_y;
	glfwGetCursorPos(wnd, &mouse_x, &mouse_y);
	update_mouse_delta(mouse_x, mouse_y, dt);

	for (auto& entry : GLFW_KEY_VIRTUAL_KEYS) {
		int action = glfwGetKey(wnd, entry.first);
		update_key(entry.second, action);
	}
}

void InputManager::update_mouse_delta(double x, double y, double dt) {
	double dx = x - m_last_mouse_x;
	m_last_mouse_x = x;
	double dy = y - m_last_mouse_y;
	m_last_mouse_y = y;

	dx /= dt;
	dy /= dt;

	m_mouse_dx = dx;
	m_mouse_dy = dy;
}

glm::vec2 InputManager::get_mouse_delta() {
	return { m_mouse_dx, m_mouse_dy };
}

void InputManager::update_key(VirtualKey v_key, int action) {
	if (action == GLFW_PRESS) {
		m_keys[v_key] = true;
	}
	else if (action == GLFW_RELEASE) {
		m_keys[v_key] = false;
	}
}

bool InputManager::get_key_state(VirtualKey v_key) {
	return m_keys[v_key];
}