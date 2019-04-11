#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

#include "cereal_glm.h"

struct Vertex {
	glm::vec3 pos;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(pos);
	}
};

struct Mesh {
	std::string name;
	std::vector<Vertex> vertexes;
	std::vector<unsigned short> indexes;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(name, vertexes, indexes);
	}
};