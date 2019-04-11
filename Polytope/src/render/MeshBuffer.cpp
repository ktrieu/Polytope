#include "MeshBuffer.h"



MeshBuffer::MeshBuffer(){
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
}


MeshBuffer::~MeshBuffer(){
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void MeshBuffer::upload_meshes(const std::vector<Mesh>& meshes) {
	int num_vertexes = 0;
	int num_indexes = 0;
	for (const Mesh& mesh : meshes) {
		MeshOffset offset;
		offset.base_vertex = num_vertexes;
		offset.index_offset = num_indexes;
		offset.index_len = mesh.indexes.size();
		m_offsets[mesh.name] = offset;
		num_vertexes += mesh.vertexes.size();
		num_indexes += mesh.indexes.size();
	}
	//allocate the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertexes, nullptr, GL_STATIC_DRAW);
	for (const Mesh& mesh : meshes) {
		MeshOffset& offset = m_offsets[mesh.name];
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * offset.base_vertex, sizeof(Vertex) * mesh.vertexes.size(),
			mesh.vertexes.data());
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * num_indexes, nullptr, GL_STATIC_DRAW);
	for (const Mesh& mesh : meshes) {
		MeshOffset& offset = m_offsets[mesh.name];
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(unsigned short) * offset.index_offset, 
			sizeof(unsigned short) * offset.index_len, mesh.vertexes.data());
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
