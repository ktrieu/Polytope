#pragma once

#include <input/VirtualKey.h>

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

class App;

class InputManager {

public:
	InputManager(App& app);
	~InputManager();

	void update(double dt);

	bool get_key_state(VirtualKey key);
	glm::vec2 get_mouse_delta();


private:
	void update_key(VirtualKey v_key, int action);
	void update_mouse_delta(double x, double y, double dt);

	App& m_app;

	double m_last_mouse_x = 0;
	double m_last_mouse_y = 0;
	double m_mouse_dx = 0;
	double m_mouse_dy = 0;

	std::unordered_map<VirtualKey, bool> m_keys;

};