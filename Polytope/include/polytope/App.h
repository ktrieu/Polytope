#pragma once

#include <resource/ResourceLoader.h>
#include <input/InputManager.h>

struct GLFWwindow;

class App {

public:
	App();
	~App();

	bool init();
	void start();

	GLFWwindow* get_window() { return m_wnd; }
	InputManager& get_input_manager() { return m_input_manager; }
	ResourceLoader& get_resource_loader() { return m_resource_loader; }

private:
	bool init_GL();
	bool init_GLFW();


	int m_width = 1080;
	int m_height = 720;
	GLFWwindow* m_wnd = nullptr;

	InputManager m_input_manager;
	ResourceLoader m_resource_loader;


};

