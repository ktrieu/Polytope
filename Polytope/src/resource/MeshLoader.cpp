#include "MeshLoader.h"

#include <fstream>
#include <filesystem>
#include <stdexcept>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

namespace fs = std::filesystem;

MeshLoader::MeshLoader() {
}


MeshLoader::~MeshLoader() {
}

Mesh& MeshLoader::get_mesh(const std::string& name) {
	auto result = m_meshes.find(name);
	if (result != m_meshes.end()) {
		return (*result).second;
	}
	else {
		throw std::invalid_argument("Mesh " + name + " not found.");
	}
}

void MeshLoader::load_from_directory(const std::string& dir) {
	fs::recursive_directory_iterator dir_iter(dir);
	for (fs::path file : dir_iter) {
		std::ifstream f_stream(file, std::ios::binary);
		cereal::BinaryInputArchive archive(f_stream);
		auto result = m_meshes.emplace(file.replace_extension().string(), Mesh{});
		archive(result.first->second);
	}
}
