#include "InputManager.h"

#include <GLFW/glfw3.h>
#include <input/InputSubscriber.h>

InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::subscribe(InputSubscriber& subscriber) {
	m_subscribers.push_back(subscriber);
}

void InputManager::on_glfw_cursor_pos_event(double x, double y) {
	std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
	double delta_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - m_last_mouse_event_time).count();
	m_last_mouse_event_time = current_time;

	double dx = x - m_last_mouse_x;
	m_last_mouse_x = x;
	double dy = y - m_last_mouse_y;
	m_last_mouse_y = y;

	dx /= delta_ms;
	dy /= delta_ms;

	for (auto& s : m_subscribers) {
		s.get().on_mouse_move(dx, dy);
	}
}

VirtualKey keycode_to_virtual_key(int key) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		return VirtualKey::EXIT;
	case GLFW_KEY_W:
		return VirtualKey::FORWARD;
	case GLFW_KEY_A:
		return VirtualKey::LEFT;
	case GLFW_KEY_D:
		return VirtualKey::RIGHT;
	case GLFW_KEY_S:
		return VirtualKey::BACK;
	default:
		return VirtualKey::NONE;
	}
}

void InputManager::on_glfw_key_event(int key, int scancode, int action, int mods) {
	VirtualKey v_key = keycode_to_virtual_key(key);
	if (action == GLFW_PRESS) {
		for (auto& s : m_subscribers) {
			s.get().on_key_press(v_key);
		}
	}
}