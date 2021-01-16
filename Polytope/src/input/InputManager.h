#pragma once

#include <vector>
#include <chrono>

class InputSubscriber;

class InputManager {

public:
	InputManager();
	~InputManager();

	void subscribe(InputSubscriber& subscriber);

	void on_glfw_key_event(int key, int scancode, int action, int mods);
	void on_glfw_cursor_pos_event(double x, double y);

private:
	double m_last_mouse_x = 0;
	double m_last_mouse_y = 0;
	std::chrono::system_clock::time_point m_last_mouse_event_time = std::chrono::system_clock::now();


	std::vector<std::reference_wrapper<InputSubscriber>> m_subscribers;

};