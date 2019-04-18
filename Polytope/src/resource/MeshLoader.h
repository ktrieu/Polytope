#pragma once

#include <unordered_map>

#include <polytope_tools/Mesh.h>

class MeshLoader {

public:
	MeshLoader();
	~MeshLoader();

	Mesh& get_mesh(const std::string& name);

	void load_from_directory(const std::string& dir);

private:
	std::unordered_map<std::string, Mesh> m_meshes;

};

