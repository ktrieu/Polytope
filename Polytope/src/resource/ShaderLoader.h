#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include "render/ShaderProgram.h"

class ShaderLoader {

public:
	ShaderLoader();
	~ShaderLoader();

	ShaderProgram& get_shader(const std::string& name);
	
	void load_from_directory(const std::string& dir);

private:
	void load_shader(const std::filesystem::path& name);

	std::unordered_map<std::string, ShaderProgram> m_shaders;

};

