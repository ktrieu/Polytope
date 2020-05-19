#include "ResourceLoader.h"

#include <fstream>
#include <filesystem>
#include <stdexcept>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

namespace fs = std::filesystem;

ResourceLoader::ResourceLoader(const std::string& root_folder) : m_root_folder(root_folder) {
}


ResourceLoader::~ResourceLoader() {
}

Mesh& ResourceLoader::get_mesh(const std::string& name) {
	auto result = m_meshes.find(name);
	if (result != m_meshes.end()) {
		return (*result).second;
	}
	else {
		throw std::invalid_argument("Mesh " + name + " not found.");
	}
}

ShaderProgram& ResourceLoader::get_shader(const std::string& name) {
	auto result = m_shaders.find(name);
	if (result != m_shaders.end()) {
		return (*result).second;
	}
	else {
		throw std::invalid_argument("Shader " + name + " not found.");
	}
}

Material& ResourceLoader::get_material(const std::string& name) {
	auto result = m_materials.find(name);
	if (result != m_materials.end()) {
		return (*result).second;
	}
	else {
		throw std::invalid_argument("Material " + name + " not found.");
	}
}

Texture& ResourceLoader::get_texture(const std::string& name) {
	auto result = m_textures.find(name);
	if (result != m_textures.end()) {
		return (*result).second;
	}
	else {
		throw std::invalid_argument("Texture " + name + " not found.");
	}
}

void ResourceLoader::load_from_subdirectory(const std::string& dir) {
	fs::recursive_directory_iterator dir_iter(m_root_folder / dir);
	for (fs::path file : dir_iter) {
		std::string name = file.lexically_relative(m_root_folder).replace_extension().generic_string();
		if (file.extension() == ".vert" || file.extension() == ".frag") {
			load_shader(file, name);
		}
		if (file.extension() == ".mdl") {
			load_mesh(file, name);
		}
		if (file.extension() == ".tex") {
			load_texture(file, name);
		}
		if (file.extension() == ".pmat") {
			load_material(file, name);
		}
	}
}

void ResourceLoader::load_mesh(const fs::path& path, const std::string& name) {
	std::ifstream f_stream(path, std::ios::binary);
	cereal::BinaryInputArchive archive(f_stream);
	auto result = m_meshes.emplace(name, Mesh{});
	archive(result.first->second);
}

void ResourceLoader::load_shader(const fs::path& path, const std::string& name) {
	//shaders occur in multiple files so we may try to load the same one twice. check first and avoid that
	if (m_shaders.find(name) != m_shaders.end()) {
		return;
	}
	std::ifstream vert_file(fs::path(path).replace_extension(".vert"));
	std::ifstream frag_file(fs::path(path).replace_extension(".frag"));
	std::stringstream vert_stream;
	std::stringstream frag_stream;
	vert_stream << vert_file.rdbuf();
	frag_stream << frag_file.rdbuf();
	m_shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name),
		std::forward_as_tuple(vert_stream.str(), frag_stream.str()));
}

void ResourceLoader::load_texture(const fs::path& path, const std::string& name) {
	std::ifstream f_stream(path, std::ios::binary);
	cereal::BinaryInputArchive archive(f_stream);
	auto result = m_textures.emplace(name, Texture{});
	archive(result.first->second);
}

void ResourceLoader::load_material(const fs::path& path, const std::string& name) {
	std::ifstream f_stream(path, std::ios::binary);
	cereal::BinaryInputArchive archive(f_stream);
	auto result = m_materials.emplace(name, Material{});
	archive(result.first->second);
}
