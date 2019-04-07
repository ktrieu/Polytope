#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
	std::cout << "Polytope\n";
	std::ifstream test_stream("data/test.txt");
	std::cout << test_stream.rdbuf() << "\n";
}