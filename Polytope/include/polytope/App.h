#pragma once

#include <resource/ResourceLoader.h>
#include <input/InputManager.h>
#include <input/InputSubscriber.h>

struct GLFWwindow;

class App : public InputSubscriber {

public:
	App();
	~App();

	bool init();
	void start();

	InputManager& get_input_manager() { return m_input_manager; }

	void on_key_press(VirtualKey key) override;

private:
	bool init_GL();
	bool init_GLFW();


	int m_width = 720;
	int m_height = 480;
	GLFWwindow* m_wnd = nullptr;

	InputManager m_input_manager;
	ResourceLoader m_resource_loader;


};

