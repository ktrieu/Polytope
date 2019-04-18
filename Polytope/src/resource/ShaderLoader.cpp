#include "ShaderLoader.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

ShaderLoader::ShaderLoader() {
}


ShaderLoader::~ShaderLoader() {
}

ShaderProgram& ShaderLoader::get_shader(const std::string& name) {
	auto result = m_shaders.find(name);
	if (result != m_shaders.end()) {
		return (*result).second;
	}
	else {
		throw std::invalid_argument("Shader " + name + " not found.");
	}
}

void ShaderLoader::load_from_directory(const std::string& dir) {
	fs::recursive_directory_iterator dir_iter(dir);
	for (fs::path file : dir_iter) {
		fs::path name = file.replace_extension();
		//shaders are going to be split across two files, so we'll run into a given shader twice
		//since ShaderProgram takes both files, we skip a shader if we've already seen it
		if (m_shaders.find(name.string()) == m_shaders.end()) {
			load_shader(name);
		}
	}
}

void ShaderLoader::load_shader(const fs::path& name) {
	std::ifstream vert_file(fs::path(name).replace_extension(".vert"));
	std::ifstream frag_file(fs::path(name).replace_extension(".frag"));
	std::stringstream vert_stream;
	std::stringstream frag_stream;
	vert_stream << vert_file.rdbuf();
	frag_stream << frag_file.rdbuf();
	m_shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name.string()),
		std::forward_as_tuple(vert_stream.str(), frag_stream.str()));
}
