#pragma once

#include <unordered_map>
#include <string>
#include <variant>

#include <glm/glm.hpp>

#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/variant.hpp>

#include "cereal_glm.h"

typedef std::variant<float, int, std::string, glm::vec2, glm::vec3, glm::vec4> UniformType;

struct Material {
	std::string name;
	std::string texture_name;
	std::string shader_name;

	std::unordered_map<std::string, UniformType> uniforms;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(name, texture_name, shader_name, uniforms);
	}
};
