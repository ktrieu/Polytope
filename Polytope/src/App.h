#pragma once

struct GLFWwindow;

class App {

public:
	App();
	~App();

	bool init();
	void start();

private:
	bool init_GLFW();

	int m_width = 720;
	int m_height = 480;
	GLFWwindow* m_wnd = nullptr;

};

