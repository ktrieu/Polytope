#include <iostream>
#include <fstream>

#include "App.h"

int main(int argc, char** argv) {
	std::cout << "Polytope\n";
	App app;
	if (app.init() == false) {
		return 0;
	}
	try {
		app.start();
	}
	catch (std::exception& e) {
		std::cout << "EXCEPTION:\n";
		std::cout << e.what() << '\n';
	}
}