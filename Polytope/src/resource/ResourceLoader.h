#pragma once

#include <unordered_map>
#include <filesystem>

#include <polytope_tools/Mesh.h>

#include <render/ShaderProgram.h>

class ResourceLoader {

public:
	ResourceLoader(const std::string& root_folder);
	~ResourceLoader();

	Mesh& get_mesh(const std::string& name);
	ShaderProgram& get_shader(const std::string& name);

	void load_from_subdirectory(const std::string& dir);

private:
	void load_mesh(const std::filesystem::path& path, const std::string& name);
	void load_shader(const std::filesystem::path& path, const std::string& name);

	const std::filesystem::path m_root_folder;
	std::unordered_map<std::string, Mesh> m_meshes;
	std::unordered_map<std::string, ShaderProgram> m_shaders;

};

