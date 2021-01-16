#pragma once

#include <resource/ResourceLoader.h>

struct GLFWwindow;

class App {

public:
	App();
	~App();

	bool init();
	void start();

	void on_key_event(int key, int scancode, int action, int mods);
	void on_cursor_pos_event(double x, double y);

private:
	bool init_GL();
	bool init_GLFW();


	int m_width = 720;
	int m_height = 480;
	GLFWwindow* m_wnd = nullptr;

	ResourceLoader m_resource_loader;

};

