#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <polytope_tools/Mesh.h>

struct MeshOffset {
	unsigned int index_offset;
	unsigned int index_len;
	unsigned int base_vertex;
};

class MeshBuffer {

public:
	MeshBuffer();
	~MeshBuffer();

	void upload_meshes(const std::vector<Mesh>& meshes);

private:
	GLuint m_vbo = 0;
	GLuint m_ibo = 0;

	std::unordered_map<std::string, MeshOffset> m_offsets;
};

