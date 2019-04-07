#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	fs::path input_dir;
	fs::path output_dir;
	if (argc == 3) {
		input_dir = fs::path(argv[1]);
		output_dir = fs::path(argv[2]);
	}
	else {
		std::cout << "Invalid number of arguments.\n";
		std::cout << "Usage: PolyTopeTools inputDir outputDir\n";
	}
	std::cout << "PolytopeTools:\n";
	std::cout << "Writing resources to " << output_dir << "\n";
	if (!fs::is_directory(output_dir)) {
		std::error_code ec;
		//attempt to create the output directory first
		if (!fs::create_directory(output_dir, ec) || ec.value() != 0) {
			std::cout << "Error: " << output_dir << " is not a valid directory.\n";
			std::cout << "OS error code: " << ec << "\n";
			return 0;
		}
	}
	fs::recursive_directory_iterator resource_iter(input_dir);
	for (fs::path resource_in : resource_iter) {
		fs::path resource_name = resource_in.lexically_relative(input_dir);
		fs::path resource_out = output_dir / resource_name;
		//if the output doesn't exist or if the input was modified after the output file
		if (!fs::is_regular_file(resource_out) || fs::last_write_time(resource_in) > fs::last_write_time(resource_out)) {
			//copy the file
			std::cout << "Processing " << resource_name << "\n";
			fs::copy_file(resource_in, resource_out, fs::copy_options::overwrite_existing);
		} 
		else {
			std::cout << "Skipping resource " << resource_name << "\n";
		}
	}
	return 0;
}