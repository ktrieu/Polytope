#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>

#include "App.h"

int main(int argc, char** argv) {
	std::cout << "Polytope\n";
	App app;
	if (app.init() == false) {
		return 0;
	}
	app.start();
}