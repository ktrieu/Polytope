#pragma once

#include <glm/glm.hpp>

namespace glm {

	template<class Archive>
	void serialize(Archive& archive, glm::vec2& vec) {
		archive(vec.x, vec.y);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec3& vec) {
		archive(vec.x, vec.y, vec.z);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec4& vec) {
		archive(vec.x, vec.y, vec.z, vec.w);
	}

}